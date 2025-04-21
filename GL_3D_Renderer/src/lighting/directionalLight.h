#pragma once

#include "light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(const glm::vec3 direction);

	inline void setDirection(const glm::vec3& direction) { mDirection = direction; };
	inline glm::vec3 getDirection() const { return mDirection; };

	void setShaderLight(class ShaderProgram& shader, const char* uniformName) const override;
private:
	glm::vec3 mDirection;
};