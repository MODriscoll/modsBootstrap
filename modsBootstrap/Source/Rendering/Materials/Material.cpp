#include "Rendering\Materials\Material.h"

#include "Rendering\Shaders\Shader.h"
#include "Rendering\Textures\Texture2D.h"

#include <cassert>

namespace mods
{
	Material::Material()
		: m_Albedo(1.f)
		, m_Roughness(0.5f)
		, m_Fresnal(0.5f)
	{
	}

	void Material::AddMap(eMaterialMaps type, const std::shared_ptr<Texture2D>& map)
	{
		assert(type != eMaterialMaps::Count);
		m_Maps.emplace_back(type, map);
	}

	void Material::Bind(ShaderProgram& program) const
	{
		program.SetUniformValue("material.albedo", m_Albedo);
		program.SetUniformValue("material.roughness", m_Roughness);
		program.SetUniformValue("material.fresnal", m_Fresnal);

		uint32 ambc = 1, difc = 1, spcc = 1, emsc = 1, nrmc = 1;
		uint32 unit = 0;
		for (const MapPair& pair : m_Maps)
		{
			std::string key("material.");
			switch (pair.Type)
			{
				case eMaterialMaps::Ambient:
				{
					key += std::string("ambient") + std::to_string(ambc++);
					break;
				}
				case eMaterialMaps::Diffuse:
				{
					key += std::string("diffuse") + std::to_string(difc++);
					break;
				}
				case eMaterialMaps::Specular:
				{
					key += std::string("specular") + std::to_string(spcc++);
					break;
				}
				case eMaterialMaps::Emmision:
				{
					key += std::string("emmision") + std::to_string(emsc++);
					break;
				}
				case eMaterialMaps::Normal:
				{
					key += std::string("normal") + std::to_string(nrmc++);
					break;
				}
				default:
				{
					assert(false);
				}
			}

			pair.Map->Bind(unit);
			program.SetUniformValue(key, (int32)unit);
			++unit;
		}
	}
}