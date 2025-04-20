#include "pointLight.h"
#include "shaderProgram.h"


void PointLight::setShaderLight(ShaderProgram& shader, const char* uniformName) const
{
	std::string base = std::string(uniformName);

	std::string color = base + ".color";
	//std::string intensity = base + ".intensity";

	std::string position = base + ".position";
	std::string constant = base + ".constant";
	std::string linear = base + ".linear";
	std::string quadratic = base + ".quadratic";

	shader.setUniformVec3(mColor, color.c_str());
	//shader.setUniformFloat(mIntensity, intensity.c_str());
	shader.setUniformVec3(mPosition, position.c_str());
	shader.setUniformFloat(mConstant, constant.c_str());
	shader.setUniformFloat(mLinear, linear.c_str());
	shader.setUniformFloat(mQuadratic, quadratic.c_str());
};
