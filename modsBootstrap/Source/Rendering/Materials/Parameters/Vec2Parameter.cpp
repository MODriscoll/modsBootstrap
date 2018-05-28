#include "Rendering\Materials\Parameters\Vec2Parameter.h"

#include "IncludeGLFW.h"

#include <glm\common.hpp>
#include <glm\gtc\type_ptr.hpp>

namespace mods
{
	Vec2Parameter::Vec2Parameter(const glm::vec2& value)
		: Value(value)
	{

	}

	void Vec2Parameter::Bind(uint32 unit) const
	{
		glUniform2fv(m_Location, 1, glm::value_ptr(Value));
	}

	bool Vec2Parameter::IsValid() const
	{
		if (!MaterialParameter::IsValid())
			return false;

		float dot = glm::dot(Value, Value);
		return !(glm::isinf(dot) || glm::isnan(dot));
	}
}