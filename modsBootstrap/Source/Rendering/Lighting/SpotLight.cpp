#include "Rendering\Lighting\SpotLight.h"

#include <glm\geometric.hpp>
#include <glm\trigonometric.hpp>

namespace mods
{
	SpotLight::SpotLight()
		: m_Direction(0.f, 1.f, 0.f)
		, m_InnerCutoff(glm::radians(45.f))
		, m_OuterCutoff(glm::radians(70.f))
	{

	}

	void SpotLight::SetDirection(const glm::vec3& direction)
	{
		m_Direction = glm::normalize(direction);
	}

	void SpotLight::SetInnerCutoff(float cutoff)
	{
		cutoff = glm::radians(cutoff);
		m_InnerCutoff = glm::max(0.f, cutoff);

		// Assure outer is larger or equal
		m_OuterCutoff = glm::max(m_InnerCutoff, m_OuterCutoff);
	}

	void SpotLight::SetOuterCutoff(float cutoff)
	{
		cutoff = glm::radians(cutoff);
		m_OuterCutoff = glm::max(0.f, cutoff);

		// Assure inter is smaller or equal
		m_InnerCutoff = glm::min(m_InnerCutoff, m_OuterCutoff);
	}
}