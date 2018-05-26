#pragma once

#include "PointLight.h"

namespace mods
{
	class SpotLight : public PointLight
	{
	public:

		SpotLight();

	public:

		// Set this lights direction
		void SetDirection(const glm::vec3& direction);

		// Set the inner cutoff of this light (in degrees)
		void SetInnerCutoff(float cutoff);

		// Set the outer cutoff of this light (in degrees)
		void SetOuterCutoff(float cutoff);

	public:

		inline virtual eLightType GetLightType() const override { return eLightType::Spot; }

		// Get this lights direction
		inline const glm::vec3& GetDirection() const { return m_Direction; }

		// Get this lights inner cutoff
		inline float GetInnerCutoff() const { return m_InnerCutoff; }

		// Get this lights outer cutoff
		inline float GetOuterCutoff() const { return m_OuterCutoff; }

	protected:

		// Direction this light is facing
		glm::vec3 m_Direction;

		// Inner cutoff of this light (in radians)
		float m_InnerCutoff;

		// Outer cutoff of this light (in radians)
		float m_OuterCutoff;
	};
}