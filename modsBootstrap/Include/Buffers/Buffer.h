#pragma once

#include "Types.h"

namespace mods
{
	enum class eBufferType : uint32
	{
		Array			= 0x8892,
		ElementArray	= 0x8893,
		Uniform			= 0x8A11
	};

	enum class eBufferUsagePattern : uint32
	{
		StreamDraw		= 0x88E0,
		StreamRead		= 0x88E1,
		StreamCopy		= 0x88E2,

		StaticDraw		= 0x88E4,
		StaticRead		= 0x88E5,
		StaticCopy		= 0x88E6,

		DynamicDraw		= 0x88E8,
		DynamicRead		= 0x88E9,
		DynamicCopy		= 0x88EA
	};
}