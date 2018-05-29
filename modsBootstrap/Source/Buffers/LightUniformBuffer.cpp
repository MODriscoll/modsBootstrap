#include "Buffers\LightUniformBuffer.h"

#include "Rendering\Lighting\LightRenderData.h"

namespace mods
{
	// struct directionallight
	// vec4 color				(16 bytes)	(offset=0)
	// float ambientstrength	(4 bytes)	(offset=16)
	// float diffusestrength	(4 bytes)	(offset=20)
	// vec3 direction			(16 bytes)	(offset=24)

	// final size				40 bytes

	// struct pointlight			
	// vec4 color				(16 bytes)	(offset=0)
	// float ambientstrength	(4 bytes)	(offset=16)
	// float diffusestrength	(4 bytes)	(offset=20)
	// vec3 position			(16 bytes)	(offset=24)
	// float constant			(4 bytes)	(offset=40)
	// float linear				(4 bytes)	(offset=44)
	// float quadratic			(4 bytes)	(offset=48)

	// final size				52 bytes

	// struct spotlight	
	// vec4 color				(16 bytes)	(offset=0)
	// float ambientstrength	(4 bytes)	(offset=16)
	// float diffusestrength	(4 bytes)	(offset=20)
	// vec3 position			(16 bytes)	(offset=24)
	// vec3 direction			(16 bytes)	(offset=40)
	// float innercutoff		(4 bytes)	(offset=56)
	// float outercutoff		(4 bytes)	(offset=60)
	// float constant			(4 bytes)	(offset=64)
	// float linear				(4 bytes)	(offset=68)
	// float quadratic			(4 bytes)	(offset=72)

	// final size				76 bytes

	// Light Uniform							binding = 1
	// directionallight dirlights[4]			(160 bytes)	(offset=0)
	// pointlight pntlights[10]					(520 bytes) (offset=160)
	// spotlight sptlights[10]					(760 bytes) (offset=680)
	// int dircount								(4 bytes)	(offset=1440)
	// int pntcount								(4 bytes)	(offset=1444)
	// int sptcount								(4 bytes)	(offset=1448)

	// final size								1452 bytes

	LightUniforms::LightUniforms()
		: UniformBuffer(1452, 1)
	{
		// We start with no lights
		int32 counts[3] = { 0, 0, 0 };
		Fill(1440, 12, counts);
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
		int32 count = index + 1;
		Fill(1440, 4, &count);

		return index;
	}

	int32 LightUniforms::AddPointLight(const PointLightData& data)
	{
		if (m_PointLights.size() >= 4)
			return -1;

		// Record index
		int32 index = m_PointLights.size();

		m_PointLights.push_back(data);

		// Fill in the data for this light
		Fill(160 + sizeof(PointLightData) * index, sizeof(PointLightData), (void*)&data);

		// Update amount of lights
		int32 count = index + 1;
		Fill(1444, 4, &count);

		return index;
	}

	int32 LightUniforms::AddSpotLight(const SpotLightData& data)
	{
		if (m_SpotLights.size() >= 4)
			return -1;

		// Record index
		int32 index = m_SpotLights.size();

		m_SpotLights.push_back(data);

		// Fill in the data for this light
		Fill(680 + sizeof(SpotLightData) * index, sizeof(SpotLightData), (void*)&data);

		// Update amount of lights
		int32 count = index + 1;
		Fill(1448, 4, &count);

		return index;
	}
}