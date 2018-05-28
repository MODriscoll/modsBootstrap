#include "Rendering\Materials\Parameters\Vec4Parameter.h"

#include "IncludeGLFW.h"

#include <glm\common.hpp>
#include <glm\gtc\type_ptr.hpp>

namespace mods
{
	Vec4Parameter::Vec4Parameter(const glm::vec4& value)
		: Value(value)
	{

	}

	void Vec4Parameter::Bind(uint32 unit) const
	{
		glUniform4fv(m_Location, 1, glm::value_ptr(Value));
	}

	bool Vec4Parameter::IsValid() const
	{
		if (!MaterialParameter::IsValid())
			return false;

		float dot = glm::dot(Value, Value);
		return !(glm::isinf(dot) || glm::isnan(dot));
	}
}