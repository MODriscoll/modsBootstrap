#pragma once

#include "Types.h"

namespace mods
{
	namespace utility
	{
		struct TextureData
		{
		public:

			TextureData()
				: Pixels(nullptr)
				, Width(0)
				, Height(0)
				, Format(0)
				, InternalFormat(0)
			{

			}

		public:

			// Pixels to texture
			byte* Pixels;

			// Width of texture
			int32 Width;

			// Height of texture
			int32 Height;

			// Amount of channels in texture
			int32 Channels;

			// Format of image
			int32 Format;

			// Interal format of image
			int32 InternalFormat;
		};

		// Loads a texture from the given file path.
		bool LoadTextureFromSource(const char* path, TextureData& data, bool sRGB = false, int32 channels = 0);

		// Destroys a texture
		void DestroyTexture(byte* pixels);
	}
}