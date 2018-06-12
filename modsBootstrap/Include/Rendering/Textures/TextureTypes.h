#pragma once

#include "Types.h"

#include "IncludeGLFW.h"

namespace mods
{
	enum class eTextureChannels : int8
	{
		Unknown		= 0,

		R			= 1,
		RG			= 2,
		RGB			= 3,
		RGBA		= 4,

		Depth		= 5
	};

	enum class eTextureFormat : int32
	{
		R				= 0x1903,
		RG				= 0x8227,
		RGB				= 0x1907,
		RGBA			= 0x1908,

		R16F			= 0x822D,
		RG16F			= 0x822F,
		RGB16F			= 0x881B,
		RGBA16F			= 0x881A,

		R32F			= 0x822E,
		RG32F			= 0x8230,
		RGB32F			= 0x8815,
		RGBA32F			= 0x8814,

		Depth			= 0x1902,
		Depth16			= 0x81A5,
		Depth24			= 0x81A6,
		Depth32			= 0x81A7,

		Depth24Stencil8 = 0x88F0
	};
}