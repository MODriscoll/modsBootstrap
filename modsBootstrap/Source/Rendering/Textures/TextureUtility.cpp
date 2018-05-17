#include "Rendering\Textures\TextureUtility.h"

#include <stb\stb_image.h>

#include "IncludeGLFW.h"

namespace mods
{
	namespace utility
	{
		bool LoadTextureFromSource(const char* path, TextureData& data, bool sRGB, int32 channels)
		{
			// TODO: remove this line
			stbi_set_flip_vertically_on_load(1);

			int32 width, height, format;
			byte* pixels = stbi_load(path, &width, &height, &format, channels);

			// Image failed to load
			if (!pixels)
			{
				stbi_image_free(pixels);
				return false;
			}

			data.Pixels = pixels;
			data.Width = width;
			data.Height = height;
			data.Channels = format;

			// TODO: Check if correct
			switch (format)
			{
				case STBI_grey:
				{
					data.InternalFormat = sRGB ? GL_SRGB8 : GL_RED;
					data.Format = GL_RED;
					break;
				}
				case STBI_grey_alpha:
				{
					data.InternalFormat = sRGB ? GL_SRGB8_ALPHA8 : GL_RG;
					data.Format = GL_RG;
					break;
				}
				case STBI_rgb:
				{
					data.InternalFormat = sRGB ? GL_SRGB : GL_RGB;
					data.Format = GL_RGB;
					break;
				}
				case STBI_rgb_alpha:
				{
					data.InternalFormat = sRGB ? GL_SRGB_ALPHA : GL_RGBA;
					data.Format = GL_RGBA;
					break;
				}
			}

			return true;
		}

		void DestroyTexture(byte* pixels)
		{
			if (pixels)
				stbi_image_free(pixels);
		}
	}
}