#include "Rendering\Materials\Parameters\IntParameter.h"

#include "IncludeGLFW.h"

#include <glm\common.hpp>

namespace mods
{
	IntParameter::IntParameter(int32 value)
		: Value(value)
	{

	}

	void IntParameter::Bind(uint32 unit) const
	{
		glUniform1i(m_Location, Value);
	}
}