#include "Buffers\LightUniformBuffer.h"

#include "Rendering\Lighting\LightRenderData.h"

namespace mods
{
	LightUniforms::LightUniforms()
		: UniformBuffer(CntOffset() + sizeof(int32) * 3, 1)
	{
		// We start with no lights
		int32 counts[3] = { 0, 0, 0 };
		Fill(CntOffset(), sizeof(int32) * 3, counts);
	}

	int32 LightUniforms::AddDirectionalLight(const DirectionalLightData& data)
	{
		if (m_DirectionalLights.size() >= 4)
			return -1;

		// Record index
		int32 index = m_DirectionalLights.size();

		m_DirectionalLights.push_back(data);

		// Fill in the data for this light
		Fill(sizeof(DirectionalLightData) * index, sizeof(DirectionalLightData), (void*)&data);

		// Update amount of lights
		SetCountAt(0, index + 1);

		return index;
	}

	int32 LightUniforms::AddPointLight(const PointLightData& data)
	{
		if (m_PointLights.size() >= 10)
			return -1;

		// Record index
		int32 index = m_PointLights.size();

		m_PointLights.push_back(data);

		// Fill in the data for this light
		Fill(PntOffset() + sizeof(PointLightData) * index, sizeof(PointLightData), (void*)&data);

		// Update amount of lights
		SetCountAt(1, index + 1);

		return index;
	}

	int32 LightUniforms::AddSpotLight(const SpotLightData& data)
	{
		if (m_SpotLights.size() >= 10)
			return -1;

		// Record index
		int32 index = m_SpotLights.size();

		m_SpotLights.push_back(data);

		// Fill in the data for this light
		Fill(SptOffset() + sizeof(SpotLightData) * index, sizeof(SpotLightData), (void*)&data);

		// Update amount of lights
		SetCountAt(2, index + 1);

		return index;
	}

	void LightUniforms::UpdateDirectionalLight(const DirectionalLightData& data, int32 index)
	{
		m_DirectionalLights[index] = data;

		// Fill in the data for this light
		Fill(sizeof(DirectionalLightData) * index, sizeof(DirectionalLightData), (void*)&data);
	}

	void LightUniforms::UpdatePointLight(const PointLightData& data, int32 index)
	{
		m_PointLights[index] = data;

		// Fill in the data for this light
		Fill(PntOffset() + sizeof(PointLightData) * index, sizeof(PointLightData), (void*)&data);
	}

	void LightUniforms::UpdateSpotLight(const SpotLightData& data, int32 index)
	{
		m_SpotLights[index] = data;

		// Fill in the data for this light
		Fill(SptOffset() + sizeof(SpotLightData) * index, sizeof(SpotLightData), (void*)&data);
	}

	void LightUniforms::SetCountAt(int32 index, int32 count) const
	{
		Fill(CntOffset() + sizeof(int32) * index, sizeof(int32), &count);
	}
}