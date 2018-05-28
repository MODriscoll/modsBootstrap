#pragma once

#include "Rendering\Materials\MaterialParameter.h"

namespace mods
{
	struct FloatParameter : public MaterialParameter
	{
	public:

		FloatParameter(float value = 0.f);

	public:

		// Binds a single float value
		virtual void Bind(uint32 unit) const override;

	public:

		// Get if the value is not nan
		virtual bool IsValid() const override;

		inline virtual eMaterialParameterType GetParameterType() const { return eMaterialParameterType::Float; }

	public:

		// Value of this parameter
		float Value;
	};
}