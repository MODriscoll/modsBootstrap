#pragma once

#include "Types.h"

namespace mods
{
	enum eProcessModel : uint32
	{
		None						= 0,
		CalcTangentSpace			= 1 << 0,
		Triangulate					= 1 << 3,
		FlipUVs						= 1 << 23		
	};
}