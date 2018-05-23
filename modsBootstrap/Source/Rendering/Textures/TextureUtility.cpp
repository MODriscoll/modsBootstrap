#include "Rendering\Textures\TextureUtility.h"

#include "IncludeGLFW.h"

#include <stb\stb_image.h>

#include <iostream>

namespace mods
{
	namespace detail
	{
		bool LoadTextureFromSource(
			const std::string& path,
			TextureData& data,
			eTextureChannels channels,
			bool sRGB)
		{
			int32 width, height, format;
			byte* pixels = stbi_load(path.c_str(), &width, &height, &format, (int32)channels);

			// Image failed to load
			if (!pixels)
				return false;

			data.Pixels = pixels;
			data.Width = width;
			data.Height = height;
			data.Channels = format;

			// Update channels based of channels requested
			if (channels == eTextureChannels::Default)
				channels = (eTextureChannels)format;

			GetTextureFormats(channels, data.Format, data.InternalFormat, sRGB);

			return true;
		}

		void DestroyTexture(byte* pixels)
		{
			if (pixels)
				stbi_image_free(pixels);
		}

		void GetTextureFormats(eTextureChannels channels, int32& format, int32& intern, bool sRGB)
		{
			switch (channels)
			{
				case eTextureChannels::Red:
				{
					intern = sRGB ? GL_SRGB8 : GL_RED;
					format = GL_RED;
					break;
				}

				case eTextureChannels::RedAlpha:
				{
					intern = sRGB ? GL_SRGB8_ALPHA8 : GL_RG;
					format = GL_RG;
					break;
				}

				case eTextureChannels::RGB:
				{
					intern = sRGB ? GL_SRGB : GL_RGB;
					format = GL_RGB;
					break;
				}

				case eTextureChannels::RGBAlpha:
				{
					intern = sRGB ? GL_SRGB_ALPHA : GL_RGBA;
					format = GL_RGBA;
					break;
				}

				default:
				{
					assert(false);
				}
			}

		}

		void SetFlip(bool flip)
		{
			stbi_set_flip_vertically_on_load((int32)flip);
		}
	}
}