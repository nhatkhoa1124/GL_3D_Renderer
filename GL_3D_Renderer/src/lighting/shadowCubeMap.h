#pragma once

#include <cstdint>
#include <vector>
#include <glm.hpp>

class ShadowCubeMap
{
public:
	ShadowCubeMap();
	~ShadowCubeMap() = default;
	void init();
	void setLightPosition(const glm::vec3& lightPos) { mLightPos = lightPos; }
	std::vector<glm::mat4> getShadowTransforms() const;

	uint32_t getFrameBuffer()const { return mFBO; }
	uint32_t getDepthMap() const { return mDepthMap; }
	float getNearPlane()const { return NEAR_PLANE; }
	float getFarPlane() const { return FAR_PLANE; }
	int getShadowSize() const { return SHADOW_SIZE; }
private:
	uint32_t mFBO;
	uint32_t mDepthMap;
	glm::vec3 mLightPos;

	static constexpr float NEAR_PLANE = 1.0f;
	static constexpr float FAR_PLANE = 25.0f;
	static constexpr int SHADOW_SIZE = 1024;
};