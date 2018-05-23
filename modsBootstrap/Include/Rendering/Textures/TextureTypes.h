#pragma once

#include "Types.h"

namespace mods
{
	enum class eTextureChannels : byte
	{
		// Channels depend on texture
		Default		= 0,

		Red			= 1,
		RedAlpha	= 2,
		RGB			= 3,
		RGBAlpha	= 4,

		Count		= 4,
	};
}