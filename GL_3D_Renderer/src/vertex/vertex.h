#pragma once

#include "core.h"

namespace Model {
	struct VertexData {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};

	struct TextureData {
		GLuint id;
		std::string type;
		std::string path;
	};

}