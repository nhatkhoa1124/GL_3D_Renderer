#pragma once

#include "vertex.h"
#include "mesh.h"
#include "assimp/scene.h"

namespace Vertex {
	class Model {
	public:
		Model(const std::string& path);
		~Model() = default;
		inline void drawModel(const ShaderProgram& shader) const
		{
			for (unsigned int i = 0; i < mMeshes.size(); i++)
				mMeshes[i].drawMesh(shader);
		};
	private:
		std::vector<Mesh> mMeshes;
		std::string mDirectory;
		std::vector<TextureData> mTexturesLoaded;

		void loadModel(const std::string& path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<TextureData> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};
}