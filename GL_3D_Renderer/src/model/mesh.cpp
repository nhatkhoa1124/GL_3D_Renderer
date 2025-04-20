#include "mesh.h"
#include "core.h"

#include "vertexArray.h"
#include "vertexBuffer.h"

namespace Model {
	Mesh::Mesh(std::vector<VertexData> vertices, std::vector<uint32_t> indices, std::vector<TextureData> textures) :
		mVertices{ vertices },
		mIndices{ indices },
		mTextures{ textures }
	{
		setupMesh();
	}

	void Mesh::drawMesh(const ShaderProgram& shader) const
	{
		uint32_t diffuseNr = 1;
		uint32_t specularNr = 1;
		for (uint32_t i = 0; i < mTextures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			std::string number;
			std::string name = mTextures[i].type;
			if (name == "texture_diffuse") {
				number = std::to_string(diffuseNr++);
			}
			else if (name == "texture_specular") {
				number = std::to_string(specularNr++);
			}
			shader.setUniformInt(i, ("material." + name + number).c_str());
			glBindTexture(GL_TEXTURE_2D, mTextures[i].id);
		}
		glActiveTexture(GL_TEXTURE0);

		// draw mesh
		glBindVertexArray(mVAO);
		glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void Mesh::setupMesh()
	{
		glGenVertexArrays(1, &mVAO);
		glGenBuffers(1, &mVBO);
		glGenBuffers(1, &mEBO);

		glBindVertexArray(mVAO);

		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(VertexData), mVertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(uint32_t), mIndices.data(), GL_STATIC_DRAW);

		auto stride = sizeof(VertexData);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(VertexData, normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(VertexData, texCoord));

		glBindVertexArray(0);
	}
}