#pragma once

// TODO: find place to relocate

#include "Buffers\UniformBuffer.h"
#include "Rendering\Lighting\LightRenderData.h"

#include <vector>

namespace mods
{
	// TODO: add defines for the amount of lights there are
	// for now
	// 4 directional lights max
	// 10 point lights max
	// 10 spot lights max
	// TODO: make globals header where constants are defined?
	const int32 MAX_DIRECTIONAL_LIGHTS = 4;
	const int32 MAX_POINT_LIGHTS = 10;
	const int32 MAX_SPOT_LIGHTS = 10;

	struct LightUniforms : public UniformBuffer
	{
	public:

		LightUniforms();

	public:

		// Adds new directional light to the buffer
		int32 AddDirectionalLight(const DirectionalLightData& data);

		// Adds new point light to the buffer
		int32 AddPointLight(const PointLightData& data);

		// Adds new spot light to the buffer
		int32 AddSpotLight(const SpotLightData& data);

		// Updates an existing directional light
		void UpdateDirectionalLight(const DirectionalLightData& data, int32 index);

		// Updates an existing point light
		void UpdatePointLight(const PointLightData& data, int32 index);

		// Updates an existing spot light
		void UpdateSpotLight(const SpotLightData& data, int32 index);

	public:

		inline int32 GetDirCount() const { return (int32)m_DirectionalLights.size(); }
		inline int32 GetPntCount() const { return (int32)m_PointLights.size(); }
		inline int32 GetSptCount() const { return (int32)m_SpotLights.size(); }

	private:

		// Sets count for amount of lights in uniform buffer
		void SetCountAt(int32 index, int32 count) const;

	private:

		// Offsets to first value of respective type in the buffer
		constexpr inline int32 DirOffset() const { return 0; }
		constexpr inline int32 PntOffset() const { return DirOffset() + sizeof(DirectionalLightData) * MAX_DIRECTIONAL_LIGHTS; }
		constexpr inline int32 SptOffset() const { return PntOffset() + sizeof(PointLightData) * MAX_POINT_LIGHTS; }
		
		// Offset to the light count in the buffer
		constexpr inline int32 CntOffset() const { return SptOffset() + sizeof(SpotLightData) * MAX_SPOT_LIGHTS; }

	protected:

		// All directional lights
		std::vector<DirectionalLightData> m_DirectionalLights;

		// All point lights
		std::vector<PointLightData> m_PointLights;

		// All spot lights
		std::vector<SpotLightData> m_SpotLights;
	};
}