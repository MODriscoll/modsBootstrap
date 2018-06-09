#include "Rendering\Lighting\LightRenderData.h"

#include "Rendering\Lighting\DirectionalLight.h"
#include "Rendering\Lighting\SpotLight.h"

namespace mods
{
	DirectionalLightData::DirectionalLightData(const DirectionalLight& light)
		: Color(light.Color)
		, Intensity(light.GetIntensity())
		, Direction(light.GetDirection(), 0.f)
	{

	}

	PointLightData::PointLightData(const PointLight& light)
		: Color(light.Color)
		, Intensity(light.GetIntensity())
		, Position(light.Position)
		, Radius(light.GetRadius())
	{

	}

	SpotLightData::SpotLightData(const SpotLight& light)
		: Color(light.Color, 1.f)
		, Position(light.Position, 1.f)
		, Direction(light.GetDirection(), 0.f)
		, Intensity(light.GetIntensity())
		, Radius(light.GetRadius())
		, InnerCutoff(light.GetInnerCutoff())
		, OuterCutoff(light.GetOuterCutoff())
	{

	}
}