#pragma once

#include "Types.h"

#include "Rendering\Textures\Texture2D.h"

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

	enum class eMaterialTypes : byte
	{
		Float,
		Int32,
		Texture2D,
		Cubemap,

		Count
	};

	// First iteration of material
	// TODO: make material/shader editor, which will allow
	// parameters to be set in the shader, the material will
	// hold the names and types of those paramters
	class Material
	{
	public:

		Material();
		Material(const Material& rhs) = delete;
		Material(Material&& rhs) = default;

		~Material() = default;

		Material& operator = (const Material& rhs) = delete;
		Material& operator = (Material&& rhs) = default;

	public:

		// Sets the map according to its types
		// By calling this function you give up ownership of the texture
		void SetMap(eMaterialMaps type, Texture2D&& map);

	public:

		// Binds this material for use in the given program
		// Assumes the given shader is already active
		void Bind(const ShaderProgram& program) const;

	private:

		// Texture maps of this material
		// TODO: update, since one texture could
		// be used for multiple maps. current implementation
		// requires a unique texture for each map, meaning duplicates
		Texture2D m_Maps[(byte)eMaterialMaps::Count];

		// Surface properties of this material
		float m_Shininess;
		float m_Opacity;
		float m_Roughness;
	};
}