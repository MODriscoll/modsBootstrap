#pragma once

#include "Types.h"

#include <glm\vec4.hpp>

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

		// Set this lights ambient strength
		void SetAmbientStrength(float strength);

		// Set this lights diffuse strength
		void SetDiffuseStrength(float strength);

	public:

		inline virtual eLightType GetLightType() const = 0;

		// Get the lights ambient strength
		inline float GetAmbientStrength() const { return m_AmbientStrength; }

		// Get the lights specular strength
		inline float GetDiffuseStrength() const { return m_DiffuseStrength; }

	public:

		// Color of this light
		glm::vec4 Color;

	protected:

		// Lights ambience strength
		float m_AmbientStrength;

		// Lights diffuse strength
		float m_DiffuseStrength;
	};
}