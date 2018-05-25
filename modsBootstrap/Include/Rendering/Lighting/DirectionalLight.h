#pragma once

#include "Light.h"

#include <glm\vec3.hpp>

namespace mods
{
	class DirectionalLight : public Light
	{
	public:

		DirectionalLight();

	public:

		// Set this lights direction
		void SetDirection(const glm::vec3& direction);

	public:

		inline virtual eLightType GetLightType() const override { return eLightType::Directional; }

		// Get this lights direction
		inline const glm::vec3& GetDirection() const { return m_Direction; }

	protected:

		// Direction this light is facing
		glm::vec3 m_Direction;
	};
}