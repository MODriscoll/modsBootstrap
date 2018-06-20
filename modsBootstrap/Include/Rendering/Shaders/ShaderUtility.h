#pragma once

#include "Types.h"

#include <string>
#include <vector>

namespace mods
{
	namespace detail
	{ 
		// Loads a shader from the given file
		bool LoadShaderFromSource(const std::string& path, std::string& out_shader);

		// Creates and compiles a shader script of given type
		// Takes in optional path to print source of error if compilation fails
		bool CreateAndCompileShader(const std::string& script, uint32 type, uint32& shader, const std::string& path = std::string());

		// Creates and links a new shader program from given shaders
		// Will destroy the shaders if bDestroy is true
		bool CreateAndLinkProgram(const std::vector<uint32>& shaders, uint32& program, bool bDestroy = true);
	}
}