#include "spotLight.h"
#include "shaderProgram.h"

SpotLight::SpotLight(const glm::vec3& position, const glm::vec3& spotDirection, const float innerCutOff, const float outerCutOff) :
	mPosition{ position },
	mSpotDirection{ spotDirection },
	mInnerCutOff{ innerCutOff },
	mOuterCutOff{ outerCutOff }
{
	mType = LightType::SPOT;
};

void SpotLight::setShaderLight(ShaderProgram& shader, const char* uniformName) const
{
	std::string base = std::string(uniformName);

	std::string color = base + ".color";
	std::string intensity = base + ".intensity";

	std::string position = base + ".position";
	std::string spotDirection = base + ".spotDirection";
	std::string innerCutOff = base + ".innerCutOff";
	std::string outerCutOff = base + ".outerCutOff";

	shader.setUniformVec3(mColor, color.c_str());
	shader.setUniformFloat(mIntensity, intensity.c_str());
	shader.setUniformVec3(mPosition, position.c_str());
	shader.setUniformVec3(mSpotDirection, spotDirection.c_str());
	shader.setUniformFloat(mInnerCutOff, innerCutOff.c_str());
	shader.setUniformFloat(mOuterCutOff, outerCutOff.c_str());
};
