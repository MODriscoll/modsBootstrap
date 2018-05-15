#pragma once

#include "IncludeGLFW.h"

#include <string>
#include <vector>

namespace mods
{
	constexpr unsigned int InvalidShader = 0;
	constexpr unsigned int InvalidProgram = 0;

	enum class mdShaderType : unsigned int
	{
		Vertex				= GL_VERTEX_SHADER,
		TessControl			= GL_TESS_CONTROL_SHADER,
		TessEvaluation		= GL_TESS_EVALUATION_SHADER,
		Geometry			= GL_GEOMETRY_SHADER,
		Fragment			= GL_FRAGMENT_SHADER,

		Compute				= GL_COMPUTE_SHADER
	};
}