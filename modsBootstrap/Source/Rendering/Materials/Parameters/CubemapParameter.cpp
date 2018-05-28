#include "Rendering\Materials\Parameters\CubemapParameter.h"

#include "IncludeGLFW.h"
#include "Rendering\Textures\Cubemap.h"

namespace mods
{
	CubemapParameter::CubemapParameter(Cubemap* value)
		: Value(value)
	{

	}

	void CubemapParameter::Bind(uint32 unit) const
	{
		Value->Bind(unit);
	}

	bool CubemapParameter::IsValid() const
	{
		return Value != nullptr && MaterialParameter::IsValid();
	}
}