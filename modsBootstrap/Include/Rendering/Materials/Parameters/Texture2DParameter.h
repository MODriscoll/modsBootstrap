#pragma once

#include "Rendering\Materials\MaterialParameter.h"

namespace mods
{
	class Texture2D;

	struct Texture2DParameter : public MaterialParameter
	{
	public:

		Texture2DParameter(Texture2D* value = nullptr);

	public:

		// Binds texture at given unit
		virtual void Bind(uint32 unit) const override;

	public:

		// Get if the texture is valid
		virtual bool IsValid() const override;

		inline virtual eMaterialParameterType GetParameterType() const { return eMaterialParameterType::Texture2D; }

	public:

		// Value of this parameter
		Texture2D* Value;
	};
}