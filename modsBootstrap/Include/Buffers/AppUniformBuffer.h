#pragma once

// TODO: find place to relocate

#include "Buffers\UniformBuffer.h"

#include <glm\vec2.hpp>

namespace mods
{
	struct AppUniforms : public UniformBuffer
	{
	public:

		AppUniforms();

	public:

		// Updates the application uniform buffer
		void UpdateBuffer(float time, const glm::ivec2& viewportsize);
	};
}