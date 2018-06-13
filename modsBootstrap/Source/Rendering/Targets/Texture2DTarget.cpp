#include "Rendering\Targets\Texture2DTarget.h"

#include "IncludeGLFW.h"

namespace mods
{
	bool Texture2DTarget::Create(int32 width, int32 height, 
		eTargetFormat format, eFilterMode filter, eWrapMode wrap)
	{
		if (width <= 0 || height <= 0)
			return false;

		// Destroy potential existing buffer
		Destroy();

		glGenTextures(1, &m_Handle);
		glBindTexture(GL_TEXTURE_2D, m_Handle);

		// TODO: mipmap option

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int32)filter);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int32)filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int32)wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int32)wrap);

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