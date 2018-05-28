#pragma once

// TODO: find place to relocate

#include "Buffers\UniformBuffer.h"

namespace mods
{
	class Camera;

	struct CameraUniforms : public UniformBuffer
	{
	public:

		CameraUniforms();
		
	public:

		// Updates the camera uniform buffer
		void UpdateBuffer(const Camera& camera) const;
	};
}