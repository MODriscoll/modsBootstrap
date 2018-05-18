#pragma once

#include "Mesh.h"

#include <string>

namespace mods
{
	class Model
	{
	public:

		Model(const std::string& path);
		Model(const Model& rhs) = delete;
		Model(Model&& rhs) = default;

		~Model() = default;

		Model& operator = (const Model& rhs) = delete;
		Model& operator = (Model&& rhs) = default;
	
	public:

		// Draw the model using given shader program
		void Draw(ShaderProgram& program) const;

	private:

		// Loads and generates the meshes from the given file
		void LoadModel(const std::string& path);

	private:

		// Meshes of this model
		std::vector<Mesh> m_Meshes;
		
		// TODO: remove?
		std::string m_Directory;
	};
}