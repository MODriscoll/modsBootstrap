#pragma once

#include "Texture.h"

namespace mods
{
	// Wrapper for a two dimensional 2D texture
	class Texture2D : public Texture
	{
	public:

		Texture2D();
		Texture2D(const std::string& path, eTextureChannels channels = eTextureChannels::Auto, bool sRGB = false);
		Texture2D(const Texture2D& rhs) = delete;
		Texture2D(Texture2D&& rhs);

		virtual ~Texture2D() = default;

		Texture2D& operator = (const Texture2D& rhs) = delete;
		Texture2D& operator = (Texture2D&& rhs);

	public:

		// Loads a new texture from the given path
		virtual bool Load(const std::string& path, eTextureChannels channels = eTextureChannels::Auto, bool sRGB = false);

		// Creates a texture with given size and format 
		virtual bool Create(int32 width, int32 height, eTextureFormat format, byte* pixels = nullptr);

		// Creates a float texture with given size and format
		virtual bool CreateF(int32 width, int32 height, eTextureFormat format, float* pixels = nullptr);

	public:

		inline int32 GetWidth() const { return m_Width; }
		inline int32 GetHeight() const { return m_Height; }

		inline eTextureChannels GetChannels() const { return m_Channels; }

		virtual uint32 GetTextureType() const override;

	protected:

		// Width and height of the texture
		int32 m_Width, m_Height;

		// Channels of this texture
		eTextureChannels m_Channels;
	};
}