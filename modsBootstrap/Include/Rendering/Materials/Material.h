#pragma once

#include "Types.h"

namespace mods
{
	class Texture;
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
	class Material
	{
	public:

		Material();
		~Material();

	public:

		void Bind(ShaderProgram* program);

		void Unbind();

	private:

		Texture* m_AmbientMap;
		Texture* m_DiffuseMap;
		Texture* m_SpecularMap;
		Texture* m_EmmisionMap;
		Texture* m_NormalMap;

		float m_Shininess;
		float m_Opacity;
		float m_Roughness;
	};
}