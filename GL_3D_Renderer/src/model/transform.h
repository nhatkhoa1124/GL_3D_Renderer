#pragma once

#include "core.h"

class Transform {
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	Transform();
	~Transform();

};