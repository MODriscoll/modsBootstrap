#pragma once

#include "Types.h"

#include "Rendering\Textures\Texture2D.h"

#include <memory>
#include <vector>

namespace mods
{
	class ShaderProgram;

	enum class eMaterialMaps : byte
	{
		Ambient,
		Diffuse,
		Specular,
		Emmision,
		Normal,

		Count
	};

	// First iteration of material
	// TODO: make material/shader editor, which will allow
	// parameters to be set in the shader, the material will
	// hold the names and types of those paramters
	class Material
	{
		struct MapPair
		{
			MapPair(eMaterialMaps type, const std::shared_ptr<Texture2D>& map)
				: Type(type)
				, Map(map)
			{

			}

			eMaterialMaps Type;
			std::shared_ptr<Texture2D> Map;
		};

	public:

		Material();

	public:

		// Adds a new map to this material
		void AddMap(eMaterialMaps type, const std::shared_ptr<Texture2D>& map);

	public:

		// Binds this material for use in the given program
		// Assumes the given shader is already active
		void Bind(ShaderProgram& program) const;

	private:

		std::vector<MapPair> m_Maps;

		// Surface properties of this material
		float m_Albedo;
		float m_Roughness;
		float m_Fresnal;
	};
}