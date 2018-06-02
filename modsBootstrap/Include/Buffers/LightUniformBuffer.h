#pragma once

// TODO: find place to relocate

#include "Buffers\UniformBuffer.h"
#include "Rendering\Lighting\LightRenderData.h"

#include <vector>

namespace mods
{
	struct LightUniforms : public UniformBuffer
	{
	public:

		LightUniforms();

	public:

		// Adds new directional light to the buffer
		int32 AddDirectionalLight(const DirectionalLightData& data);

		// Adds new point light to the buffer
		int32 AddPointLight(const PointLightData& data);

		// Adds new spot light to the buffer
		int32 AddSpotLight(const SpotLightData& data);

	protected:

		// All directional lights
		std::vector<DirectionalLightData> m_DirectionalLights;

		// All point lights
		std::vector<PointLightData> m_PointLights;

		// All spot lights
		std::vector<SpotLightData> m_SpotLights;
	};
}