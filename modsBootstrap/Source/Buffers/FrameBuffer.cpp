#include "Buffers\FrameBuffer.h"

#include "IncludeGLFW.h"

#include <cassert>
#include <iostream>

namespace mods
{
	FrameBuffer::FrameBuffer()
		: m_FBO(0)
		, m_RBO(0)
	{
		
	}

	FrameBuffer::FrameBuffer(FrameBuffer&& rhs)
		: m_FBO(rhs.m_FBO)
		, m_RBO(rhs.m_RBO)
	{
		rhs.m_FBO = 0;
		rhs.m_RBO = 0;
	}

	FrameBuffer::~FrameBuffer()
	{
		// TODO: unload func
		if (IsValid())
		{
			glDeleteFramebuffers(1, &m_FBO);
			glDeleteRenderbuffers(1, &m_RBO);
		}
	}

	FrameBuffer& FrameBuffer::operator=(FrameBuffer&& rhs)
	{
		// Clear potential existing buffer
		// TODO: unload func
		if (IsValid())
		{
			glDeleteFramebuffers(1, &m_FBO);
			glDeleteRenderbuffers(1, &m_RBO);
		}

		m_FBO = rhs.m_FBO;
		m_RBO = rhs.m_RBO;

		rhs.m_FBO = 0;
		rhs.m_RBO = 0;

		return *this;
	}

	bool FrameBuffer::Create(int32 width, int32 height, std::vector<eTextureBufferTypes> types)
	{
		// for now
		if (IsValid())
			return false;

		if (types.empty())
			return false;

		if (width <= 0 || height <= 0)
			return false;

		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		std::vector<uint32> attachments;

		// Create textures based on their internal format
		for (int32 i = 0; i < types.size(); ++i)
		{
			Texture2D tex;
			tex.Create(width, height, (eTextureChannels)types[i]);

			attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, tex.GetHandle(), 0);

			m_Textures.push_back(std::move(tex));
		}

		glDrawBuffers(attachments.size(), attachments.data());

		glGenRenderbuffers(1, &m_RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

		uint32 result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (result != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Error: Frame buffer status was not complete after setup. Reason: " << GetStringForError(result) << std::endl;
			glDeleteFramebuffers(1, &m_FBO);
			glDeleteRenderbuffers(1, &m_RBO);
			m_Textures.clear();
			return false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return true;
	}

	void FrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	}

	void FrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
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