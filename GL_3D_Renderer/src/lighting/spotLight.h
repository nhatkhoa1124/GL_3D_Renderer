#pragma once

#include "light.h"

class SpotLight : public Light
{
public:
	SpotLight(const glm::vec3& position, const glm::vec3& spotDirection, const float innerCutOff, const float outerCutOff) :
		mPosition{ position },
		mSpotDirection{ spotDirection },
		mInnerCutOff{ innerCutOff },
		mOuterCutOff{ outerCutOff }
	{
	};

	inline void setSpotDirection(const glm::vec3& spotDirection) { mSpotDirection = spotDirection; };
	inline void setCutOff(const float innerCutOff, const float outerCutOff)
	{
		mInnerCutOff = innerCutOff;
		mOuterCutOff = outerCutOff;
	};

	inline glm::vec3 getPosition() const { return mPosition; };

	void setShaderLight(class ShaderProgram& shader, const char* uniformName) const override;
private:
	glm::vec3 mPosition, mSpotDirection;
	float mInnerCutOff, mOuterCutOff;
};