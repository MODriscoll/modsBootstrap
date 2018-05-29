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

			if (channels == eTextureChannels::Unknown)
				channels = (eTextureChannels)format;

			GetTextureFormats(channels, data.Format, data.InternalFormat, sRGB);

			return true;
		}

		void DestroyTexture(byte* pixels)
		{
			if (pixels)
				stbi_image_free(pixels);
		}

		uint32 GetDataFormat(eTextureFormat intern)
		{
			switch (intern)
			{
				case eTextureFormat::R:
				case eTextureFormat::R16F:
				{
					return GL_R16;
				};

				case eTextureFormat::RG:
				case eTextureFormat::RG16F:
				{
					return GL_RG;
				}

				case eTextureFormat::RGB:
				case eTextureFormat::RGB16F:
				{
					return GL_RGB;
				}

				case eTextureFormat::RGBA:
				case eTextureFormat::RGBA16F:
				{
					return GL_RGBA;
				}

				default:
				{
					assert(false);
				}
			}
		}

		eTextureChannels GetTextureChannels(uint32 format)
		{
			switch (format)
			{
				case GL_RED: { return eTextureChannels::R; }
				case GL_RG: { return eTextureChannels::RG; }
				case GL_RGB: { return eTextureChannels::RGB; }
				case GL_RGBA: { return eTextureChannels::RGBA; }
			}

			return eTextureChannels::Unknown;
		}

		void GetTextureFormats(eTextureChannels channels, uint32& format, int32& intern, bool sRGB)
		{
			switch (channels)
			{
				case eTextureChannels::R:
				{
					intern = sRGB ? GL_SRGB8 : GL_RED;
					format = GL_RED;
					break;
				}

				case eTextureChannels::RG:
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

				case eTextureChannels::RGBA:
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