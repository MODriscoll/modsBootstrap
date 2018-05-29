#pragma once

#include "Types.h"

namespace mods
{
	enum class eTextureChannels : int8
	{
		Unknown		= 0,

		R			= 1,
		RG			= 2,
		RGB			= 3,
		RGBA		= 4,

		Count		= 4,
	};

	enum class eTextureFormat : int32
	{
		R16F			= 0x822D,
		RG16F			= 0x822F,
		RGB16F			= 0x881B,
		RGBA16F			= 0x881A,

		R				= 0x1903,
		RG				= 0x8227,
		RGB				= 0x1907,
		RGBA			= 0x1908
	};
}