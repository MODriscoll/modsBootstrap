#pragma once

#include "Types.h"

#include <string>

namespace mods
{
	namespace detail
	{ 
		// Loads a shader from the given file
		bool LoadShaderFromSource(const std::string& path, std::string& out_shader);
	}
}