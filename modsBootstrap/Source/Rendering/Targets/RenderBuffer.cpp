#include "Rendering\Targets\RenderBuffer.h"

#include "IncludeGLFW.h"

namespace mods
{
	bool RenderBuffer::Create(int32 width, int32 height, eTargetFormat format)
	{
		if (width <= 0 || height <= 0)
			return false;

		// Destroy potential existing buffer
		Destroy();

		glGenRenderbuffers(1, &m_Handle);
		glBindRenderbuffer(GL_RENDERBUFFER, m_Handle);

		glRenderbufferStorage(GL_RENDERBUFFER, (int32)format, width, height);

		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		m_Format = format;

		return true;
	}

	bool RenderBuffer::Destroy()
	{
		if (IsValid())
			glDeleteRenderbuffers(1, &m_Handle);

		m_Handle = 0;
		m_Format = eTargetFormat::R;

		return true;
	}

	bool RenderBuffer::BoundToFrameBuffer(int32 index) const
	{
		bool iscolor;
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GetAttachmentType(index, iscolor), GL_RENDERBUFFER, m_Handle);

		return iscolor;
	}
}