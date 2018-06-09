#include "Rendering\Lighting\PointLight.h"

#include <glm\common.hpp>

namespace mods
{
	PointLight::PointLight()
		: Position(0.f)
		, m_Radius(10.f)
	{

	}

	void PointLight::SetRadius(float radius)
	{
		m_Radius = glm::max(radius, 0.001f);
	}
}