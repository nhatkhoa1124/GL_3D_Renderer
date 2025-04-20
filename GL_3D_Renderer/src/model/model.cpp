#include "model.h"
#include "core.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "stb_image.h"

namespace Model {
	void checkGLError(const char* functionName) {
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cout << "OpenGL error in " << functionName << ": " << err << std::endl;
		}
	}

	GLuint TextureFromFile(const char* path, const std::string& directory, bool setFlipTexture = false, bool gamma = false)
	{
		std::string filename = std::string(path);
		filename = directory + '/' + filename;
		stbi_set_flip_vertically_on_load(setFlipTexture);

		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}

	Model::Model(const std::string& path, bool flipTexture) :
		mFlipTexture{ flipTexture }
	{
		std::cout << "Loading model from: " << path << std::endl;
		loadModel(path);
	}

	void Model::loadModel(const std::string& path)
	{
		std::cout << "Starting model loading process..." << std::endl;

		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);


		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR: ASSIMP: " << importer.GetErrorString() << std::endl;
			return;
		}

		std::cout << "Model loaded SUCCESSFULLY: " << path << std::endl;
		mDirectory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);

	};

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		checkGLError("processNode_____________________________");
		std::cout << "Processing node: " << node->mName.C_Str() << std::endl;
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			std::cout << "Processing mesh: " << mesh->mName.C_Str() << std::endl;
			mMeshes.push_back(processMesh(mesh, scene));
		}

		for (uint32_t i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	};

	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::cout << "Processing mesh data..." << std::endl;

		std::vector<VertexData> vertices;
		std::vector<uint32_t> indices;
		std::vector<TextureData> textures;

		// Process vertices
		std::cout << "Processing vertices..." << std::endl;
		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			VertexData vertex;
			glm::vec3 vector;

			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;

			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;


			if (mesh->mTextureCoords[0]) {
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.texCoord = vec;
			}
			else {
				vertex.texCoord = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(vertex);
		}

		// Process indices
		std::cout << "Processing indices..." << std::endl;
		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// Load material textures
		if (mesh->mMaterialIndex >= 0)
		{
			std::cout << "Loading material textures..." << std::endl;
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			// Diffuse maps
			std::vector<TextureData> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			// Specular maps
			std::vector<TextureData> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		}

		std::cout << "Mesh processed SUCCESSFULLY!" << std::endl;
		return Mesh{ vertices, indices, textures };
	};

	std::vector<TextureData> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::cout << "Loading material textures of type: " << typeName << std::endl;
		std::vector<TextureData> textures;

		for (uint32_t i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skip = false;
			for (uint32_t j = 0; j < mTexturesLoaded.size(); j++)
			{
				if (std::strcmp(mTexturesLoaded[j].path.data(), str.C_Str()) == 0)
				{
					textures.push_back(mTexturesLoaded[j]);
					skip = true;
					break;
				}
			}

			if (!skip)
			{
				TextureData texture;
				texture.id = TextureFromFile(str.C_Str(), mDirectory, mFlipTexture);
				texture.type = typeName;
				texture.path = str.C_Str();
				textures.push_back(texture);
				mTexturesLoaded.push_back(texture);
				std::cout << "Loaded new texture: " << str.C_Str() << std::endl;
			}
		}

		std::cout << "Textures of type " << typeName << " loaded SUCCESSFULLY!" << std::endl;
		return textures;
	};

}
