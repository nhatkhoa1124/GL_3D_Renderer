#include "directionalLight.h"
#include "shaderProgram.h"

DirectionalLight::DirectionalLight(const glm::vec3 direction) :
	mDirection{ direction }
{
	mType = LightType::DIRECTIONAL;
};

void DirectionalLight::setShaderLight(ShaderProgram& shader, const char* uniformName) const
{
	std::string base = std::string(uniformName);

	std::string color = base + ".color";
	//std::string intensity = base + ".intensity";

	std::string direction = base + ".direction";

	shader.setUniformVec3(mColor, color.c_str());
	//shader.setUniformFloat(mIntensity, intensity.c_str());
	shader.setUniformVec3(mDirection, direction.c_str());
};
