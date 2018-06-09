#include "Rendering\Lighting\Light.h"

#include <glm\common.hpp>

namespace mods
{
	Light::Light()
		: Color(1.f)
		, m_Intensity(10.f)
		, m_AmbientStrength(0.5f)
		, m_DiffuseStrength(0.5f)
	{

	}

	void Light::SetIntensity(float intensity)
	{
		m_Intensity = glm::max(intensity, 0.f);
	}

	void Light::SetAmbientStrength(float strength)
	{
		m_AmbientStrength = glm::clamp(strength, 0.f, 1.f);
	}

	void Light::SetDiffuseStrength(float strength)
	{
		m_DiffuseStrength = glm::clamp(strength, 0.f, 1.f);
	}
}