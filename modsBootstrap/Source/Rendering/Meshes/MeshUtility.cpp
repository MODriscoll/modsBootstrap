#include "Rendering\Meshes\MeshUtility.h"

#include "IncludeGLFW.h"
#include "Rendering\Materials\Material.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <iostream>
#include <unordered_map>

namespace mods
{
	namespace detail
	{
		// TODO: Comment

		glm::vec2 ToVec2(aiVector3D& vec) { return glm::vec2(vec.x, vec.y); }
		glm::vec3 ToVec3(aiVector3D& vec) { return glm::vec3(vec.x, vec.y, vec.z); }

		// Converts an assimp mesh to a usable mesh
		// Emplaces the mesh to end of the given meshes array
		void Convert(
			aiMesh& mesh, 
			const aiScene* scene, 
			const std::string& directory, 
			std::vector<Mesh>& meshes);

		//// Loads the textures for the given type from the material
		//// Only loads a texture if it doesn't exist
		//template <aiTextureType aiType, eMaterialTextureType modType>
		//void LoadTexturesFromMaterial(
		//	aiMaterial* material,
		//	const std::string& directory,
		//	std::vector<MeshTexture>& textures,
		//	std::unordered_map<std::string, uint32>& table);

		void LoadMapFromMaterial(
			aiMaterial* from,
			Material& to,
			aiTextureType t1,
			eMaterialMaps t2,
			const std::string& directory,
			std::vector<std::shared_ptr<Texture2D>>& maps,
			std::unordered_map<std::string, uint32>& table);

		bool LoadModelFromSource(const std::string& path, ModelData& data, uint32 procflags)
		{
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path, procflags);

			// Model failed to load
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				std::cout << "Error: Failed to open model source at path: " << path 
					<< "\nError Description: " << importer.GetErrorString() << std::endl;

				return false;
			}

			std::string directory = path.substr(0, path.find_last_of('/'));

			data.Meshes.reserve(scene->mNumMeshes);

			aiMesh** meshes = scene->mMeshes;

			std::vector<aiNode*> nodes(1, scene->mRootNode);
			for (uint32 i = 0; i < nodes.size(); ++i)
			{
				aiNode* node = nodes[i];
				uint32 numMeshes = node->mNumMeshes;
				for (uint32 j = 0; j < numMeshes; ++j)
				{
					aiMesh* mesh = meshes[node->mMeshes[j]];
					Convert(*mesh, scene, directory, data.Meshes);
				}

				nodes.insert(nodes.cend(), node->mChildren, node->mChildren + node->mNumChildren);
			}

			return true;
		}

		void Convert(aiMesh& mesh, const aiScene* scene, const std::string& directory, std::vector<Mesh>& meshes)
		{
			std::vector<MeshVertex> vertices;
			std::vector<uint32> indices;
			Material material;

			vertices.reserve(mesh.mNumVertices);
			vertices.reserve(mesh.mNumFaces * 3u);


			// Convert all the meshes vertices
			for (uint32 i = 0; i < mesh.mNumVertices; ++i)
			{
				MeshVertex vertex;

				vertex.Position = ToVec3(mesh.mVertices[i]);
				vertex.Normal = ToVec3(mesh.mNormals[i]);
				
				if (mesh.HasTextureCoords(0))
					vertex.TexCoords = ToVec2(mesh.mTextureCoords[0][i]);

				vertices.emplace_back(std::move(vertex));
			}

			// Convert all the meshes faces
			for (uint32 i = 0; i < mesh.mNumFaces; ++i)
			{
				const aiFace& face = mesh.mFaces[i];
				indices.insert(indices.cend(), face.mIndices, face.mIndices + face.mNumIndices);
			}

			// TODO: update once materials have been implemented
			if (scene->HasMaterials())
			{
				aiMaterial* mat = scene->mMaterials[mesh.mMaterialIndex];

				std::vector<std::shared_ptr<Texture2D>> maps;
				std::unordered_map<std::string, uint32> table;

				LoadMapFromMaterial(mat, material, aiTextureType::aiTextureType_DIFFUSE, eMaterialMaps::Diffuse, directory, maps, table);
				LoadMapFromMaterial(mat, material, aiTextureType::aiTextureType_SPECULAR, eMaterialMaps::Specular, directory, maps, table);

				/*LoadTexturesFromMaterial<aiTextureType::aiTextureType_DIFFUSE, eMaterialTextureType::Diffuse>
					(material, directory, textures, table);
				LoadTexturesFromMaterial<aiTextureType::aiTextureType_SPECULAR, eMaterialTextureType::Specular>
					(material, directory, textures, table);*/
			}

			#if _DEBUG
			const char* name = mesh.mName.C_Str();
			if (!mesh.HasPositions())
				std::cout << "Warning: Mesh '" << name << "' has no vertex positions" << std::endl;
			if (!mesh.HasNormals())
				std::cout << "Warning: Mesh '" << name << "' has no face normals" << std::endl;
			if (!mesh.HasFaces())
				std::cout << "Warning: Mesh '" << name << "' has no faces" << std::endl;
			#endif

			meshes.emplace_back(vertices, indices, material);
		}

		//template <aiTextureType aiType, eMaterialTextureType modType>
		//void LoadTexturesFromMaterial(aiMaterial* material, const std::string& directory, 
		//	std::vector<MeshTexture>& textures, std::unordered_map<std::string, uint32>& table)
		//{
		//	uint32 count = material->GetTextureCount(aiType);
		//	for (uint32 i = 0; i < count; ++i)
		//	{
		//		aiString temp;
		//		material->GetTexture(aiType, i, &temp);

		//		//std::string path(temp.C_Str());

		//		auto it = table.find(temp.C_Str());
		//		if (it != table.cend())
		//		{
		//			textures.push_back(textures[it->second]);
		//		}
		//		else
		//		{
		//			std::string path(directory);
		//			path.append("/");
		//			path.append(temp.C_Str());

		//			TextureData data;				
		//			if (LoadTextureFromSource(path, data))
		//			{
		//				uint32 index = textures.size();

		//				MeshTexture texture;

		//				// TODO: make utility function (with options)
		//				glGenTextures(1, &texture.Handle);
		//				glBindTexture(GL_TEXTURE_2D, texture.Handle);

		//				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//				
		//				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//				glTexImage2D(GL_TEXTURE_2D, 0, data.InternalFormat, data.Width,
		//					data.Height, 0, data.Format, GL_UNSIGNED_BYTE, data.Pixels);
		//				glGenerateMipmap(GL_TEXTURE_2D);

		//				glBindTexture(GL_TEXTURE_2D, 0);

		//				texture.Type = modType;

		//				textures.push_back(texture);
		//				table[temp.C_Str()] = index;
		//			}
		//		}
		//	}
		//}

		void LoadMapFromMaterial(aiMaterial* from, Material& to, aiTextureType t1, eMaterialMaps t2, const std::string& directory,
			std::vector<std::shared_ptr<Texture2D>>& maps, std::unordered_map<std::string, uint32>& table)
		{
			uint32 count = from->GetTextureCount(t1);
			for (uint32 i = 0; i < count; ++i)
			{
				aiString temp;
				from->GetTexture(t1, i, &temp);

				//std::string path(temp.C_Str());

				auto it = table.find(temp.C_Str());
				if (it != table.cend())
				{
					to.AddMap(t2, maps[it->second]);
				}
				else
				{
					std::string path(directory);
					path.append("/");
					path.append(temp.C_Str());

					std::shared_ptr<Texture2D> map = std::make_shared<Texture2D>(path);
					to.AddMap(t2, map);
					table[temp.C_Str()] = maps.size();
					maps.push_back(map);
				}
			}
		}
	}
}