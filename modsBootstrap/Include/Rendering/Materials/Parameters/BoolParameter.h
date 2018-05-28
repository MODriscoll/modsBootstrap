#pragma once

#include "Rendering\Materials\MaterialParameter.h"

namespace mods
{
	struct BoolParameter : public MaterialParameter
	{
	public:

		BoolParameter(bool value = 0);

	public:

		// Binds a boolean value (as an int)
		virtual void Bind(uint32 unit) const override;

	public:

		inline virtual eMaterialParameterType GetParameterType() const { return eMaterialParameterType::Bool; }

	public:

		// Value of this parameter
		bool Value;
	};
}