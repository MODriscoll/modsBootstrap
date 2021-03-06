#pragma once

#include "Mesh.h"

#include <string>

namespace mods
{
	namespace detail
	{
		// TODO: update mesh to be more derive friendly
		// TODO: make skeleton inherit from mesh

		struct ModelData
		{
		public:

			// Meshes of the model
			std::vector<Mesh> Meshes;
		};

		// Loads a model from the given file path
		// TODO: make enum for proc flags
		bool LoadModelFromSource(
			const std::string& path, 
			ModelData& data, 
			eProcessModel process = eProcessModel::Triangulate);
	}
}