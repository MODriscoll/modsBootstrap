#pragma once

#include "IncludeGLFW.h"

namespace mods
{
	// Remove
	constexpr unsigned int InvalidShader = 0;
	constexpr unsigned int InvalidProgram = 0;

	enum class eShaderType : unsigned int
	{
		Vertex				= GL_VERTEX_SHADER,
		TessControl			= GL_TESS_CONTROL_SHADER,
		TessEvaluation		= GL_TESS_EVALUATION_SHADER,
		Geometry			= GL_GEOMETRY_SHADER,
		Fragment			= GL_FRAGMENT_SHADER,

		Compute				= GL_COMPUTE_SHADER
	};

	// Rename and maybe move
	enum class eShaderCreation : char
	{
		Vertex			= 1 << 0,
		TessControl		= 1 << 1,
		TessEvaluation	= 1 << 2,
		Geometry		= 1 << 3,
		Fragment		= 1 << 4
	};
}