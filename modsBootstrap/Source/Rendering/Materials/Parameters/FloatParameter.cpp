#include "Rendering\Materials\Parameters\FloatParameter.h"

#include "IncludeGLFW.h"

#include <glm\common.hpp>

namespace mods
{
	FloatParameter::FloatParameter(float value)
		: Value(value)
	{

	}

	void FloatParameter::Bind(uint32 unit) const
	{
		glUniform1f(m_Location, Value);
	}

	bool FloatParameter::IsValid() const
	{
		return !(glm::isinf(Value) || glm::isnan(Value)) && MaterialParameter::IsValid();
	}
}