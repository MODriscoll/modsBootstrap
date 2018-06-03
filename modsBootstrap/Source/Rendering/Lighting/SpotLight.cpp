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
		m_InnerCutoff = glm::cos(glm::radians(cutoff));
	}

	void SpotLight::SetOuterCutoff(float cutoff)
	{
		m_OuterCutoff = glm::cos(glm::radians(cutoff));
	}
}