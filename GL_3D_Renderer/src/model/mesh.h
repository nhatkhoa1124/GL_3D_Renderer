#pragma once

#include "vertex.h"
#include "shaderProgram.h"

namespace Model {
	class Mesh {
	public:
		Mesh(std::vector<VertexData> vertices, std::vector<uint32_t> indices, std::vector<TextureData> textures);
		~Mesh() = default;
		void drawMesh(const ShaderProgram& shader) const;
	private:
		std::vector<VertexData> mVertices;
		std::vector<uint32_t> mIndices;
		std::vector<TextureData> mTextures;

		GLuint mVAO, mVBO, mEBO;
		void setupMesh();
	};

}