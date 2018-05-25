#include "Rendering\Lighting\DirectionalLight.h"

#include <glm\geometric.hpp>

namespace mods
{
	DirectionalLight::DirectionalLight()
		: m_Direction(0.f, 1.f, 0.f)
	{

	}

	void DirectionalLight::SetDirection(const glm::vec3& direction)
	{
		m_Direction = glm::normalize(direction);
	}
}