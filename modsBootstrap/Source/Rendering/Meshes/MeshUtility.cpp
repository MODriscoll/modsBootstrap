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

		// Loads the maps for a given type from the given material
		void LoadMapFromMaterial(
			aiMaterial* from,
			Material& to,
			aiTextureType t1,
			eMaterialMaps t2,
			const std::string& directory,
			std::vector<std::shared_ptr<Texture2D>>& maps,
			std::unordered_map<std::string, uint32>& table);

		bool LoadModelFromSource(const std::string& path, ModelData& data, eProcessModel process)
		{
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path, (uint32)process);

			// Model failed to load
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				std::cout << "Error: Failed to open model source at path: " << path 
					<< "\nError Description: " << importer.GetErrorString() << std::endl;

				return false;
			}

			// Store directory as we will need it for material maps
			std::string directory = path.substr(0, path.find_last_of('/'));

			// Reserve what we need before we start
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

			// Reserve what we need before we start
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

				if (mesh.HasTangentsAndBitangents())
				{
					vertex.Tangent = ToVec3(mesh.mTangents[i]);
					vertex.Bitangent = ToVec3(mesh.mBitangents[i]);
				}

				vertices.emplace_back(std::move(vertex));
			}

			// Convert all the meshes faces
			for (uint32 i = 0; i < mesh.mNumFaces; ++i)
			{
				const aiFace& face = mesh.mFaces[i];
				indices.insert(indices.cend(), face.mIndices, face.mIndices + face.mNumIndices);
			}

			// Convert the material
			if (scene->HasMaterials())
			{
				aiMaterial* mat = scene->mMaterials[mesh.mMaterialIndex];

				// Prevent loading multiple of the same textures
				std::vector<std::shared_ptr<Texture2D>> maps;
				std::unordered_map<std::string, uint32> table;

				LoadMapFromMaterial(mat, material, aiTextureType::aiTextureType_DIFFUSE, eMaterialMaps::Diffuse, directory, maps, table);
				LoadMapFromMaterial(mat, material, aiTextureType::aiTextureType_SPECULAR, eMaterialMaps::Specular, directory, maps, table);
				LoadMapFromMaterial(mat, material, aiTextureType::aiTextureType_NORMALS, eMaterialMaps::Normal, directory, maps, table);
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

		void LoadMapFromMaterial(aiMaterial* from, Material& to, aiTextureType t1, eMaterialMaps t2, const std::string& directory,
			std::vector<std::shared_ptr<Texture2D>>& maps, std::unordered_map<std::string, uint32>& table)
		{
			uint32 count = from->GetTextureCount(t1);
			for (uint32 i = 0; i < count; ++i)
			{
				aiString temp;
				from->GetTexture(t1, i, &temp);

				std::string path(temp.C_Str());

				// Don't need to create a new texture
				// if it has already been loaded
				auto it = table.find(path);
				if (it != table.cend())
				{
					to.AddMap(t2, maps[it->second]);
				}
				else
				{
					// Will need to start from root of directory
					std::string fullpath(directory);
					fullpath.append("/");
					fullpath.append(path);

					std::shared_ptr<Texture2D> map = std::make_shared<Texture2D>(fullpath);
					to.AddMap(t2, map);

					// Set index before adding to vector,
					// so index is not out of range
					table[path] = maps.size();
					maps.push_back(map);
				}
			}
		}
	}
}