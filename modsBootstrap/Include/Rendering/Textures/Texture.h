#pragma once

#include "TextureTypes.h"

#include <string>

namespace mods
{
	// Base class for types that wrap an opengl texture
	class Texture
	{
	public:

		Texture();
		Texture(const Texture& rhs) = delete;
		Texture(Texture&& rhs);

		virtual ~Texture();

		Texture& operator = (const Texture& rhs) = delete;
		Texture& operator = (Texture&& rhs);

	public:

		// Destroys the existing texture if any
		virtual bool Destroy();

	public:

		// Binds this texture for use at the given slot
		void Bind(uint32 slot = 0);

		// Unbinds this texture from use
		void Unbind();

	public:

		inline bool IsValid() const { return m_Handle != 0; }

		inline uint32 GetHandle() const { return m_Handle; }

	protected:

		// Gets this textures opengl type
		virtual uint32 GetTextureType() const = 0;
		
	protected:

		// Handle to the texture
		uint32 m_Handle;
	};
}