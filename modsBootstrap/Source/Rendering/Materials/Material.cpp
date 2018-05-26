#include "Rendering\Materials\Material.h"

#include "Rendering\Shaders\Shader.h"
#include "Rendering\Textures\TextureUtility.h"

#include <cassert>

namespace mods
{
	Material::Material()
		: m_Shininess(32.f)
		, m_Opacity(1.f)
		, m_Roughness(0.5f)
	{
	}

	void Material::SetMap(eMaterialMaps type, Texture2D&& map)
	{
		assert(type != eMaterialMaps::Count);
		m_Maps[(byte)type] = std::move(map);
	}

	void Material::Bind(const ShaderProgram& program) const
	{
		for (byte i = 0; i < (byte)eMaterialMaps::Count; ++i)
		{
			const Texture2D& map = m_Maps[i];
			map.Bind(i);
		}
	}
}