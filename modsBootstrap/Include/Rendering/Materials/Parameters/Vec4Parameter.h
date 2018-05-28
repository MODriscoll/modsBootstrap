#pragma once

#include "Rendering\Materials\MaterialParameter.h"

#include <glm\vec4.hpp>

namespace mods
{
	struct Vec4Parameter : public MaterialParameter
	{
	public:

		Vec4Parameter(const glm::vec4& value = glm::vec4(0.f));

	public:

		// Binds four float values
		virtual void Bind(uint32 unit) const override;

	public:

		// Get if the vector is not nan
		virtual bool IsValid() const override;

		inline virtual eMaterialParameterType GetParameterType() const { return eMaterialParameterType::Vec4; }

	public:

		// Value of this parameter
		glm::vec4 Value;
	};
}