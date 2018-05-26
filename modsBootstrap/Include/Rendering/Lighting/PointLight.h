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

		inline virtual eLightType GetLightType() const override { return eLightType::Point; }

	public:

		// Position of this light
		glm::vec3 Position;

		// Attenuation of this light
		LightAttenuation Attenuation;
	};
}