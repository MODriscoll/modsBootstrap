#pragma once

#include "Types.h"

#include <glm\vec3.hpp>
#include <glm\vec4.hpp>

namespace mods
{
	class DirectionalLight;
	class PointLight;
	class SpotLight;

	// Structs used by the renderer to store lighting data
	struct DirectionalLightData
	{
	public:

		DirectionalLightData() = default;
		DirectionalLightData(const DirectionalLight& light);

	public:

		// Color of the light
		glm::vec4 Color;

		// Ambient strength of the light
		float AmbientStrength;

		// Diffuse strength of the light
		float DiffuseStrength;

	private:

		// Padding variables
		float Padding1;
		float Padding2;

	public:

		// Direction of the light
		glm::vec4 Direction;
	};

	struct PointLightData
	{
	public:

		PointLightData() = default;
		PointLightData(const PointLight& light);

	public:

		// Color of the light
		glm::vec4 Color;

		// Ambient strength of the light
		float AmbientStrength;

		// Diffuse strength of the light
		float DiffuseStrength;

	private:

		// Padding variables
		float Padding1;
		float Padding2;

	public:

		// Position of the light
		glm::vec3 Position;

		// Constant of the attenuation
		float Constant;

		// Linear of the attenuation
		float Linear;

		// Quadratic of the attenuation
		float Quadratic;

	private:

		// Padding variables
		float Padding3;
		float Padding4;
	};

	struct SpotLightData
	{
	public:

		SpotLightData() = default;
		SpotLightData(const SpotLight& light);

	public:

		// Color of the light
		glm::vec4 Color;

		// Ambient strength of the light
		float AmbientStrength;

		// Diffuse strength of the light
		float DiffuseStrength;

	private:

		// Padding variables
		float Padding1;
		float Padding2;

	public:

		// Position of the light
		glm::vec4 Position;

		// Direction of the light
		glm::vec3 Direction;

		// Inner cutoff of the light
		float InnerCutoff;

		// Outer cutoff of the light
		float OuterCutoff;

		// Constant of the attenuation
		float Constant;

		// Linear of the attenuation
		float Linear;

		// Quadratic of the attenuation
		float Quadratic;
	};
}