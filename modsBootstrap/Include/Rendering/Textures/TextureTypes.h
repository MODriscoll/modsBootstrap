#pragma once

#include "Types.h"

namespace mods
{
	enum class eTextureChannels : int8
	{
		Auto		= 0,

		R			= 1,
		RG			= 2,
		RGB			= 3,
		RGBA		= 4,
	};

	enum class eTextureWrap : int32
	{
		Repeat		= 0x2901,
		Mirror		= 0x8370,
		Edge		= 0x812F,
		Border		= 0x812D
	};

	enum class eTextureFilter : int32
	{
		Nearest			= 0x2600,
		Linear			= 0x2601

		// TODO: mipmaps
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