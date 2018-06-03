#include "Buffers\LightUniformBuffer.h"

#include "Rendering\Lighting\LightRenderData.h"

namespace mods
{
	// STRUCTS get padded to be the multiplication of a vec4 (vec4 being 16 bytes)

	// struct directionallight
	// vec4 color				(16 bytes)	(offset=0)
	// float ambientstrength	(4 bytes)	(offset=16)
	// float diffusestrength	(4 bytes)	(offset=20)
	// vec3 direction			(16 bytes)	(offset=24)

	// final size				40 bytes
	// padded size				48 bytes

	// struct pointlight			
	// vec4 color				(16 bytes)	(offset=0)
	// float ambientstrength	(4 bytes)	(offset=16)
	// float diffusestrength	(4 bytes)	(offset=20)
	// vec3 position			(16 bytes)	(offset=24)
	// float constant			(4 bytes)	(offset=40)
	// float linear				(4 bytes)	(offset=44)
	// float quadratic			(4 bytes)	(offset=48)

	// final size				52 bytes
	// padded size				64 bytes

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
	// padded size				80 bytes

	// Light Uniform							binding = 1 (using padded size)
	// directionallight dirlights[4]			(192 bytes)	(offset=0)
	// pointlight pntlights[10]					(640 bytes) (offset=192)
	// spotlight sptlights[10]					(800 bytes) (offset=832)
	// int dircount								(4 bytes)	(offset=1632)
	// int pntcount								(4 bytes)	(offset=1636)
	// int sptcount								(4 bytes)	(offset=1640)

	// final size								1644 bytes

	LightUniforms::LightUniforms()
		: UniformBuffer(1644, 1)
	{
		// We start with no lights
		int32 counts[3] = { 0, 0, 0 };
		Fill(1632, 12, counts);
	}

	int32 LightUniforms::AddDirectionalLight(const DirectionalLightData& data)
	{
		if (m_DirectionalLights.size() >= 4)
			return -1;

		// Record index
		int32 index = m_DirectionalLights.size();

		m_DirectionalLights.push_back(data);

		// Fill in the data for this light
		Fill(48 * index, sizeof(DirectionalLightData), (void*)&data);

		// Update amount of lights
		int32 count = index + 1;
		Fill(1632, 4, &count);

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
		Fill(192 + (64 * index), sizeof(PointLightData), (void*)&data);

		// Update amount of lights
		int32 count = index + 1;
		Fill(1636, 4, &count);

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
		Fill(832 + (80 * index), sizeof(SpotLightData), (void*)&data);

		// Update amount of lights
		int32 count = index + 1;
		Fill(1640, 4, &count);

		return index;
	}

	void LightUniforms::UpdateSpotLight(const SpotLightData& data, int32 index)
	{
		m_SpotLights[index] = data;

		// Fill in the data for this light
		Fill(832 + (80 * index), sizeof(SpotLightData), (void*)&data);
	}
}