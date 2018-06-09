#include "Rendering\Lighting\SpotLight.h"

#include <glm\geometric.hpp>
#include <glm\trigonometric.hpp>

namespace mods
{
	SpotLight::SpotLight()
		: m_Direction(0.f, 1.f, 0.f)
		, m_InnerCutoff(glm::radians(25.f))
		, m_OuterCutoff(glm::radians(40.f))
	{

	}

	void SpotLight::SetDirection(const glm::vec3& direction)
	{
		m_Direction = glm::normalize(direction);
	}

	void SpotLight::SetInnerCutoff(float cutoff)
	{
		// TODO: clamp so its equal or less than outer
		m_InnerCutoff = glm::cos(glm::radians(cutoff));
	}

	void SpotLight::SetOuterCutoff(float cutoff)
	{
		// TODO: clamp so its equal or greater than outer
		m_OuterCutoff = glm::cos(glm::radians(cutoff));
	}
}