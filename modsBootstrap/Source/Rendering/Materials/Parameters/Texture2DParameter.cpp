#include "Rendering\Materials\Parameters\Texture2DParameter.h"

#include "IncludeGLFW.h"
#include "Rendering\Textures\Texture2D.h"

namespace mods
{
	Texture2DParameter::Texture2DParameter(Texture2D* value)
		: Value(value)
	{

	}

	void Texture2DParameter::Bind(uint32 unit) const
	{
		Value->Bind(unit);
	}

	bool Texture2DParameter::IsValid() const
	{
		return Value != nullptr && MaterialParameter::IsValid();
	}
}