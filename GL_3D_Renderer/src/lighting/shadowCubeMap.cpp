#include "shadowCubeMap.h"
#include "glad/glad.h"
#include <gtc/matrix_transform.hpp> 

ShadowCubeMap::ShadowCubeMap() :
	mFBO{ 0 },
	mDepthMap{ 0 },
	mLightPos{ 0.0f, 0.0f, 0.0f }
{
}

void ShadowCubeMap::init()
{
	// Set cube map textures
	glGenTextures(1, &mDepthMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mDepthMap);

	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_SIZE, SHADOW_SIZE, 0,
			GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Set frame buffer for cube map
	glGenFramebuffers(1, &mFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	// Attach cube map texture to frame buffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mDepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::vector<glm::mat4> ShadowCubeMap::getShadowTransforms() const
{
	std::vector<glm::mat4> shadowTransforms;
	float aspect = 1.0f; // SHADOW_SIZE / SHADOW_SIZE = 1
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, NEAR_PLANE, FAR_PLANE);
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(mLightPos, mLightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(mLightPos, mLightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(mLightPos, mLightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(mLightPos, mLightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(mLightPos, mLightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(mLightPos, mLightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	return shadowTransforms;
}
