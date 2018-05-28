#include "Rendering\Materials\Parameters\Mat4Parameter.h"

#include "IncludeGLFW.h"

#include <glm\common.hpp>
#include <glm\gtc\type_ptr.hpp>

namespace mods
{
	Mat4Parameter::Mat4Parameter(const glm::mat4& value)
		: Value(value)
	{

	}

	void Mat4Parameter::Bind(uint32 unit) const
	{
		glUniformMatrix4fv(m_Location, 1, GL_FALSE, glm::value_ptr(Value));
	}

	bool Mat4Parameter::IsValid() const
	{
		if (!MaterialParameter::IsValid())
			return false;

		float dot = glm::determinant(Value);
		return !(glm::isinf(dot) || glm::isnan(dot));
	}
}