#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include<vector>
#include<string>
#include<assimp\Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.h"


class model
{
public:
	std::vector<texture> textures_loaded;
	std::vector<mesh> meshes;
	std::string directory;
	std::string modelname;

	model(std::string path,std::string name)
	{
		modelname = name;
		loadModel(path);
	}
	void Draw(Shader shader)
	{
		for (unsigned int i = 0; i < meshes.size(); i++)
		{
			meshes[i].Draw(shader);
		}
	}
private:
	void loadModel(std::string path)
	{
		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}
		processNode(scene->mRootNode, scene);
	}
	void processNode(aiNode *node, const aiScene *scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}
	mesh processMesh(aiMesh *aimesh, const aiScene *scene)
	{
		std::vector<vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<texture> textures;

		for (unsigned int i = 0; i < aimesh->mNumVertices; i++)
		{
			vertex temp_vertex;
			glm::vec3 temp_vec3;
			temp_vec3.x = aimesh->mVertices[i].x;
			temp_vec3.y = aimesh->mVertices[i].y;
			temp_vec3.z = aimesh->mVertices[i].z;
			temp_vertex.position = temp_vec3;

			temp_vec3.x = aimesh->mNormals[i].x;
			temp_vec3.y = aimesh->mNormals[i].z;
			temp_vec3.z = aimesh->mNormals[i].z;
			temp_vertex.normal = temp_vec3;

			if (aimesh->mTextureCoords)
			{
				glm::vec2 temp_texcoord;
				temp_texcoord.x = aimesh->mTextureCoords[0][i].x;
				temp_texcoord.y = aimesh->mTextureCoords[0][i].y;
				temp_vertex.texcoord = temp_texcoord;
			}
			else
				temp_vertex.texcoord = glm::vec2(0.0f, 0.0f);

			temp_vec3.x = aimesh->mTangents[i].x;
			temp_vec3.y = aimesh->mTangents[i].z;
			temp_vec3.z = aimesh->mTangents[i].z;
			temp_vertex.tangent = temp_vec3;

			temp_vec3.x = aimesh->mBitangents[i].x;
			temp_vec3.y = aimesh->mBitangents[i].z;
			temp_vec3.z = aimesh->mBitangents[i].z;
			temp_vertex.bitangent = temp_vec3;

			vertices.push_back(temp_vertex);
		}
		for (unsigned int i = 0; i < aimesh->mNumFaces; i++)
		{
			aiFace face = aimesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		aiMaterial* material = scene->mMaterials[aimesh->mMaterialIndex];
		std::vector<texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		std::vector<texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
		return mesh(vertices, indices, textures);
	}


	std::vector<texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
	{
		std::vector<texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skip = false;
			for (unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true;
					break;
				}
			}
			if (!skip)
			{
				texture temp_texture;
				temp_texture.id = TextureFromFile(str.C_Str());
				temp_texture.type = typeName;
				temp_texture.path = str;
				textures.push_back(temp_texture);
				textures_loaded.push_back(temp_texture);
			}
		}
		return textures;
	}
	unsigned int TextureFromFile(const char* path)
	{
		std::string filename = std::string(path);
		filename = "./Data/Models/" + modelname +"/" + filename;
		unsigned int textureid;
		glGenTextures(1, &textureid);

		int height, width, nrComponents;
		unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureid);
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

		return textureid;
	}
	
};

