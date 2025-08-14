#pragma once

#include <string>
#include <cstdint>
#include "glm.hpp"

namespace Model {

	struct VertexData {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};

	struct TextureData {
		uint32_t id;
		std::string type;
		std::string path;
	};

}