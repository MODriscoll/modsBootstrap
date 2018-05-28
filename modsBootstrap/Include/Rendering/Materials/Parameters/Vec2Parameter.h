#pragma once

#include "Rendering\Materials\MaterialParameter.h"

#include <glm\vec2.hpp>

namespace mods
{
	struct Vec2Parameter : public MaterialParameter
	{
	public:

		Vec2Parameter(const glm::vec2& value = glm::vec2(0.f));

	public:

		// Binds two float values
		virtual void Bind(uint32 unit) const override;

	public:

		// Get if the vector is not nan
		virtual bool IsValid() const override;

		inline virtual eMaterialParameterType GetParameterType() const { return eMaterialParameterType::Vec2; }

	public:

		// Value of this parameter
		glm::vec2 Value;
	};
}