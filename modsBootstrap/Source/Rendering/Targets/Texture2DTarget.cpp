#include "Rendering\Targets\Texture2DTarget.h"

#include "IncludeGLFW.h"

namespace mods
{
	bool Texture2DTarget::Create(int32 width, int32 height, eTargetFormat format)
	{
		if (width <= 0 || height <= 0)
			return false;

		// Destroy potential existing buffer
		Destroy();

		glGenTextures(1, &m_Handle);
		glBindTexture(GL_TEXTURE_2D, m_Handle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// TODO: add options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, (int32)format, width, height, 
			0, GetPixelFormat(format), GetDataType(format), nullptr);

		glBindTexture(GL_TEXTURE_2D, 0);
		
		m_Format = format;

		return true;
	}

	bool Texture2DTarget::BoundToFrameBuffer(int32 index) const
	{
		bool iscolor;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GetAttachmentType(index, iscolor), GL_TEXTURE_2D, m_Handle, 0);

		return iscolor;
	}

	uint32 Texture2DTarget::GetTextureType() const
	{
		return GL_TEXTURE_2D;
	}
}