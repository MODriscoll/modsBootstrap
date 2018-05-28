#pragma once

#include "Rendering\Materials\MaterialParameter.h"

namespace mods
{
	struct IntParameter : public MaterialParameter
	{
	public:

		IntParameter(int32 value = 0);

	public:

		// Binds a single int value
		virtual void Bind(uint32 unit) const override;

	public:

		inline virtual eMaterialParameterType GetParameterType() const { return eMaterialParameterType::Int; }

	public:

		// Value of this parameter
		int32 Value;
	};
}