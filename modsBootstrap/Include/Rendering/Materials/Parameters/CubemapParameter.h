#pragma once

#include "Rendering\Materials\MaterialParameter.h"

namespace mods
{
	class Cubemap;

	struct CubemapParameter : public MaterialParameter
	{
	public:

		CubemapParameter(Cubemap* value = nullptr);

	public:

		// Binds cubemap at given unit
		virtual void Bind(uint32 unit) const override;

	public:

		// Get if the cubemap is valid
		virtual bool IsValid() const override;

		inline virtual eMaterialParameterType GetParameterType() const { return eMaterialParameterType::Cubemap; }

	public:

		// Value of this parameter
		Cubemap* Value;
	};
}