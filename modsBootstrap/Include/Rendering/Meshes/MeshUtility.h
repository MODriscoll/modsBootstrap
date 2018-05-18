#pragma once

#include "Types.h"

#include "Mesh.h"

#include <string>

// for now, implement own flags
#include <assimp\postprocess.h>

namespace mods
{
	namespace detail
	{
		struct ModelData
		{
		public:

			ModelData()
			{

			}

		public:

			// Meshes of the model
			std::vector<Mesh> Meshes;
		};

		// Loads a model from the given file path
		// TODO: make enum for proc flags
		bool LoadModelFromSource(const std::string& path, ModelData& data, uint32 procflags);
	}
}