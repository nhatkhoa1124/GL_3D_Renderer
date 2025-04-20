#pragma once

#include "glm.hpp"

enum class LightType {
	SPOT,
	DIRECTIONAL,
	POINT
};

class Light {
public:
	Light() :
		mColor{ glm::vec3(1.0f) },
		mIntensity{ 0.5f }
	{
	};
	virtual ~Light() = default;

	virtual void setShaderLight(class ShaderProgram& shader, const char* uniformName) const = 0;

	inline void setColor(const glm::vec3& color) { mColor = color; };
	inline void setIntensity(const float intensity) { mIntensity = intensity; };

	inline glm::vec3 getColor() const { return mColor; };
	inline float getIntensity() const { return mIntensity; };
	inline LightType getLightType() const { return mType; };

protected:
	glm::vec3 mColor;
	float mIntensity;
	LightType mType;
};
