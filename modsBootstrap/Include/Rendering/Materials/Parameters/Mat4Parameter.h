#pragma once


#include "Rendering\Materials\MaterialParameter.h"

#include <glm\mat4x4.hpp>

namespace mods
{
	struct Mat4Parameter : public MaterialParameter
	{
	public:

		Mat4Parameter(const glm::mat4& value = glm::mat4(1.f));

	public:

		// Binds a 4x4 matrix
		virtual void Bind(uint32 unit) const override;

	public:

		// Get if the matrix is not nan
		virtual bool IsValid() const override;

		inline virtual eMaterialParameterType GetParameterType() const { return eMaterialParameterType::Mat4; }

	public:

		// Value of this parameter
		glm::mat4 Value;
	};
}