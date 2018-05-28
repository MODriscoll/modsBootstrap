#include "Rendering\Materials\Parameters\BoolParameter.h"

#include "IncludeGLFW.h"

namespace mods
{
	BoolParameter::BoolParameter(bool value)
		: Value(value)
	{

	}

	void BoolParameter::Bind(uint32 unit) const
	{
		glUniform1i(m_Location, (int32)Value);
	}
}