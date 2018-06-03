#pragma once

#include "Types.h"

namespace mods
{
	struct LightAttenuation
	{
	public:

		LightAttenuation()
			: Constant(1.f)
			, Linear(0.022f)
			, Quadratic(0.0019f)
		{

		}

	public:

		inline bool IsValid() const
		{
			return (Constant >= 0.f) && (Linear >= 0.f) && (Quadratic >= 0.f);
		}

	public:

		// Constant of the attenuation.
		// Usually set to a value of 1
		float Constant;

		// Linear factor of the attenuation
		// Describes lights range before fading
		float Linear;

		// Quadratic (exponential) factor of the attenuation
		float Quadratic;
	};
}