#pragma once

#include "Rendering\Materials\MaterialParameter.h"

#include <glm\vec3.hpp>

namespace mods
{
	struct Vec3Parameter : public MaterialParameter
	{
	public:

		Vec3Parameter(const glm::vec3& value = glm::vec3(0.f));

	public:

		// Binds three float values
		virtual void Bind(uint32 unit) const override;

	public:

		// Get if the vector is not nan
		virtual bool IsValid() const override;

		inline virtual eMaterialParameterType GetParameterType() const { return eMaterialParameterType::Vec3; }

	public:

		// Value of this parameter
		glm::vec3 Value;
	};
}