#pragma once

#include "Types.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace mods
{
	namespace detail
	{
		struct GlyphData
		{
		public:

			GlyphData()
			{

			}

		public:


		};

		// Initialises the free type library
		void InitFreetype();

		// Cleans up the free type library
		void CleanFreetype();

		// Loads 
	}
}