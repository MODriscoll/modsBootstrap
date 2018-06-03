#pragma once

#include "Mesh.h"

#include <string>

namespace mods
{
	class Model
	{
	public:

		Model() = default;
		Model(const std::string& path, eProcessModel process = eProcessModel::Triangulate);
		Model(const Model& rhs) = delete;
		Model(Model&& rhs) = default;

		~Model() = default;

		Model& operator = (const Model& rhs) = delete;
		Model& operator = (Model&& rhs) = default;
	
	public:

		// Loads and generates the meshes from the given file
		bool Load(const std::string& path, eProcessModel process = eProcessModel::Triangulate);

		// Draw the model using given shader program
		void Draw(ShaderProgram& program) const;

	public:

		inline bool IsValid() const { return !m_Meshes.empty(); }

	private:

		// Meshes of this model
		std::vector<Mesh> m_Meshes;
		
		// TODO: remove?
		std::string m_Directory;
	};
}