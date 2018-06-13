#pragma once

#include "Types.h"

namespace mods
{
	enum class eProcessModel : uint32
	{
		None						= 0,
		CalcTangentSpace			= 1,
		Triangulate					= 8,
		GenNormals					= 32,
		FlipUVs						= 8388608	
	};

	inline constexpr eProcessModel operator | (eProcessModel lhs, eProcessModel rhs)
	{
		return static_cast<eProcessModel>(static_cast<uint32>(lhs) | static_cast<uint32>(rhs));
	}
}