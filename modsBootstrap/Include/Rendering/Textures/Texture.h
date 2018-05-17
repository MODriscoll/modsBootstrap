#pragma once

#include "Types.h"

#include <string>

namespace mods
{
	//enum class eTextureChannels : int
	//{

	//};

	//enum class eTextureWrapping : int
	//{

	//};

	class Texture
	{
	public:

		Texture(const std::string& path);
		Texture(const Texture& rhs) = delete;
		Texture(Texture&& rhs);

		virtual ~Texture();

		Texture& operator = (const Texture& rhs) = delete;
		Texture& operator = (Texture&& rhs);

	public:

		// Loads a new texture from the given path.
		// Existing texture is destroyed
		bool Load(const std::string& path);

		// Destroys and invalidates this texture
		bool Unload();

		// Binds this texture for use at the given slot
		void Bind(uint32 slot = 0);

		// Unbinds this texture from use
		void Unbind();

	public:

		inline bool IsValid() const { return m_Handle != 0; }

		inline int32 GetWidth() const { return m_Width; }
		inline int32 GetHeight() const { return m_Height; }

		// Get the pixels of this texture.
		// This value can be null
		inline const byte* GetPixels() const { return m_Pixels; }

	private:

		// Handle to the texture
		uint32 m_Handle;

		// Pixels of the loaded texture
		byte* m_Pixels;

		// Width and height of the texture
		int32 m_Width, m_Height;

		// Color channels of the texture
		int32 m_Channels;
	};
}