#pragma once

#include "light.h"

class PointLight : public Light
{
public:
	PointLight(const glm::vec3& position, const float constant, const float linear, const float quadratic) :
		mPosition{ position },
		mConstant{ constant },
		mLinear{ linear },
		mQuadratic{ quadratic }
	{
	};

	inline void setPosition(const glm::vec3& position) { mPosition = position; };
	inline void setAttenuation(const float constant, const float linear, const float quadratic)
	{
		mConstant = constant;
		mLinear = linear;
		mQuadratic = quadratic;
	};

	inline glm::vec3 getPosition() const { return mPosition; };

	void setShaderLight(class ShaderProgram& shader, const char* uniformName) const override;
private:
	glm::vec3 mPosition;
	float mConstant, mLinear, mQuadratic;
};

