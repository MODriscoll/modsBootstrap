#pragma once

#include "Types.h"

#include <glm\vec3.hpp>

namespace mods
{
	enum class eLightType : byte
	{
		Point,
		Spot,
		Directional,

		Count
	};

	class Light
	{
	public:

		Light();
		virtual ~Light() = default;

	public:

		// Set this lights intensity
		void SetIntensity(float intensity);

		// Set this lights ambient strength
		void SetAmbientStrength(float strength);

		// Set this lights diffuse strength
		void SetDiffuseStrength(float strength);

	public:

		inline virtual eLightType GetLightType() const = 0;

		// Get this lights intensity
		inline float GetIntensity() const { return m_Intensity; }

		// Get the lights ambient strength
		inline float GetAmbientStrength() const { return m_AmbientStrength; }

		// Get the lights specular strength
		inline float GetDiffuseStrength() const { return m_DiffuseStrength; }

	public:

		// Color of this light
		glm::vec3 Color;

	protected:

		// The intensity of this light
		float m_Intensity;

		// Lights ambience strength
		float m_AmbientStrength;

		// Lights diffuse strength
		float m_DiffuseStrength;
	};
}