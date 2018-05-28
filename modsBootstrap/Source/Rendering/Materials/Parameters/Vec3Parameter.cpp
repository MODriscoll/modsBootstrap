#include "Rendering\Materials\Parameters\Vec3Parameter.h"

#include "IncludeGLFW.h"

#include <glm\common.hpp>
#include <glm\gtc\type_ptr.hpp>

namespace mods
{
	Vec3Parameter::Vec3Parameter(const glm::vec3& value)
		: Value(value)
	{

	}

	void Vec3Parameter::Bind(uint32 unit) const
	{
		glUniform3fv(m_Location, 1, glm::value_ptr(Value));
	}

	bool Vec3Parameter::IsValid() const
	{
		if (!MaterialParameter::IsValid())
			return false;

		float dot = glm::dot(Value, Value);
		return !(glm::isinf(dot) || glm::isnan(dot));
	}
}