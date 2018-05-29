#pragma once

#include "TextureTypes.h"

#include <string>

namespace mods
{
	namespace detail
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
			uint32 Format;

			// Interal format of image
			int32 InternalFormat;
		};

		// Loads a texture from the given file path.
		bool LoadTextureFromSource(
			const std::string& path, 
			TextureData& data, 
			eTextureChannels channels, 
			bool sRGB);

		// Destroys a texture
		void DestroyTexture(byte* pixels);

		// Gets the data format associated with the internal format of a texture
		uint32 GetDataFormat(eTextureFormat intern);

		// Get the amount of channels based on the format of a texture
		eTextureChannels GetTextureChannels(uint32 format);

		// Get the formatting for a texture based on its channels
		void GetTextureFormats(
			eTextureChannels channels, 
			uint32& format, 
			int32& intern, 
			bool sRGB);

		// TODO: replace, have a struct to pass that takes in loading options
		// TODO: add a generate texture handle here too
		void SetFlip(bool flip);
	}
}