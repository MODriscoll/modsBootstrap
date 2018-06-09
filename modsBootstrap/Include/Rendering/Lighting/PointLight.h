#pragma once

#include "Light.h"
#include "LightAttenuation.h"

#include <glm\vec3.hpp>

namespace mods
{
	class PointLight : public Light
	{
	public:

		PointLight();

	public:

		// Set this lights radius
		void SetRadius(float radius);

	public:

		inline virtual eLightType GetLightType() const override { return eLightType::Point; }

		// Get this lights radius
		inline float GetRadius() const { return m_Radius; }

	public:

		// Position of this light
		glm::vec3 Position;

	protected:

		// Radius of this light
		float m_Radius;

		// Attenuation of this light
		LightAttenuation Attenuation;
	};
}