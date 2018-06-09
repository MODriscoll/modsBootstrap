#pragma once

#include "Types.h"

#include <glm\vec3.hpp>
#include <glm\vec4.hpp>

namespace mods
{
	class DirectionalLight;
	class PointLight;
	class SpotLight;

	// These structs are aligned to so the can be
	// written to a uniform buffer with one call.
	// Each struct size (sizeof()) is a multiplication of 16

	struct DirectionalLightData
	{
	public:

		DirectionalLightData() = default;
		DirectionalLightData(const DirectionalLight& light);

	protected:

		// Color of the light (in linear space)
		glm::vec3 Color;

		// Intensity of this light
		float Intensity;

		// Direction of the light
		glm::vec4 Direction;
	};

	struct PointLightData
	{
	public:

		PointLightData() = default;
		PointLightData(const PointLight& light);

	protected:

		// Color of the light (in linear space)
		glm::vec3 Color;

		// Intensity of this light
		float Intensity;

		// Position of the light
		glm::vec3 Position;	

		// Radius of this light
		float Radius;
	};

	struct SpotLightData
	{
	public:

		SpotLightData() = default;
		SpotLightData(const SpotLight& light);

	protected:

		// Color of the light (in linear space)
		glm::vec4 Color;

		// Position of the light
		glm::vec4 Position;

		// Direction of this light
		glm::vec4 Direction;

		// Intensity of this light
		float Intensity;

		// Radius (length) of this light
		float Radius;

		// Inner cutoff of this light
		float InnerCutoff;
		
		// Outer cutoff of this light
		float OuterCutoff;
	};
}