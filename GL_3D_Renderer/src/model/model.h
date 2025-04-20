#pragma once

#include "vertex.h"
#include "mesh.h"
#include "assimp/scene.h"

namespace Model {
	class Model {
	public:
		Model(const std::string& path, bool flipTexture);
		~Model() = default;
		inline void drawModel(const ShaderProgram& shader) const
		{
			for (unsigned int i = 0; i < mMeshes.size(); i++)
				mMeshes[i].drawMesh(shader);
		};
		inline void setFlipTexture(bool flip) { mFlipTexture = flip; };
	private:
		std::vector<Mesh> mMeshes;
		std::string mDirectory;
		std::vector<TextureData> mTexturesLoaded;
		bool mFlipTexture;

		void loadModel(const std::string& path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<TextureData> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};
}