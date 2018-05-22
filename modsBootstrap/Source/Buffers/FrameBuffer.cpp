#include "Buffers\FrameBuffer.h"

#include "IncludeGLFW.h"

#include <cassert>
#include <iostream>

namespace mods
{
	FrameBuffer::FrameBuffer(int32 width, int32 height)
		: m_FBO(0)
	{
		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		uint32 

		uint32 result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (result != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Error: Frame buffer status was not complete after setup. Reason: " << GetStringForError(result) << std::endl;
			glDeleteFramebuffers(1, &m_FBO);
			assert(false);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	FrameBuffer::FrameBuffer(FrameBuffer&& rhs)
		: m_FBO(rhs.m_FBO)
	{
		rhs.m_FBO = 0;
	}

	FrameBuffer::~FrameBuffer()
	{
		// TODO: unload func
		if (IsValid())
			glDeleteFramebuffers(1, &m_FBO);
	}

	FrameBuffer& FrameBuffer::operator=(FrameBuffer&& rhs)
	{
		// Clear potential existing buffer
		// TODO: unload func
		if (IsValid())
			glDeleteFramebuffers(1, &m_FBO);

		m_FBO = rhs.m_FBO;

		rhs.m_FBO = 0;

		return *this;
	}

	constexpr const char* FrameBuffer::GetStringForError(uint32 error) const
	{
		switch (error)
		{
			case GL_FRAMEBUFFER_UNDEFINED: { return "Framebuffer is undefined"; }
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: { return "Attachment is incomplete"; }
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: { return "Framebuffer is missing an image attachment"; }
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: { return "Missing draw buffer"; }
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: { return "Missing read buffer"; }
			case GL_FRAMEBUFFER_UNSUPPORTED: { return "Internal format of buffer is unsupported"; }
		}

		return "Unkown Error";
	}
}