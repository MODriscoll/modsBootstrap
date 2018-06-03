#include "Rendering\Lighting\LightRenderData.h"

#include "Rendering\Lighting\DirectionalLight.h"
#include "Rendering\Lighting\SpotLight.h"

namespace mods
{
	DirectionalLightData::DirectionalLightData(const DirectionalLight& light)
		: Color(light.Color, 1.f)
		, AmbientStrength(light.GetAmbientStrength())
		, DiffuseStrength(light.GetDiffuseStrength())
		, Direction(light.GetDirection(), 0.f)
	{

	}

	PointLightData::PointLightData(const PointLight& light)
		: Color(light.Color, 1.f)
		, AmbientStrength(light.GetAmbientStrength())
		, DiffuseStrength(light.GetDiffuseStrength())
		, Position(light.Position)
		, Constant(light.Attenuation.Constant)
		, Linear(light.Attenuation.Linear)
		, Quadratic(light.Attenuation.Quadratic)
	{

	}

	SpotLightData::SpotLightData(const SpotLight& light)
		: Color(light.Color, 1.f)
		, AmbientStrength(light.GetAmbientStrength())
		, DiffuseStrength(light.GetDiffuseStrength())
		, Position(light.Position, 1.f)
		, Direction(light.GetDirection())
		, InnerCutoff(light.GetInnerCutoff())
		, OuterCutoff(light.GetOuterCutoff())
		, Constant(light.Attenuation.Constant)
		, Linear(light.Attenuation.Linear)
		, Quadratic(light.Attenuation.Quadratic)
	{

	}
}