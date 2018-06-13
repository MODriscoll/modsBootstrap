#include "Rendering\Targets\FrameBuffer.h"

#include "IncludeGLFW.h"
#include "Rendering\Targets\Texture2DTarget.h"

#include <cassert>
#include <iostream>

namespace mods
{
	FrameBuffer::FrameBuffer(uint32 width, uint32 height)
		: m_FBO(0)
		, m_Width(width)
		, m_Height(height)
	{
		assert(m_Width > 0 && m_Height > 0);
	}

	FrameBuffer::FrameBuffer(FrameBuffer&& rhs)
		: m_FBO(rhs.m_FBO)
		, m_Width(rhs.m_Width)
		, m_Height(rhs.m_Height)
		, m_Textures(std::move(rhs.m_Textures))
		, m_RenderBuffers(std::move(rhs.m_RenderBuffers))
	{
		rhs.m_FBO = 0;
		rhs.m_Width = 0;
		rhs.m_Height = 0;
	}

	FrameBuffer::~FrameBuffer()
	{
		Destroy();
	}

	FrameBuffer& FrameBuffer::operator=(FrameBuffer&& rhs)
	{
		// Destroy potential existing buffer
		Destroy();

		m_FBO = rhs.m_FBO;
		m_Width = rhs.m_Width;
		m_Height = rhs.m_Height;
		m_Textures = std::move(rhs.m_Textures);
		m_RenderBuffers = std::move(rhs.m_RenderBuffers);

		rhs.m_FBO = 0;
		rhs.m_Width = 0;
		rhs.m_Height = 0;

		return *this;
	}

	void FrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	}

	void FrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	int32 FrameBuffer::AttachTexture2D(eTargetFormat format, eFilterMode filter, eWrapMode wrap)
	{
		if (IsValid())
			return -1;

		Texture2DTarget* target = new Texture2DTarget();
		target->Create(m_Width, m_Height, format, filter, wrap);

		// Record index before adding
		int32 index = (int32)m_Textures.size();

		m_Textures.push_back(target);

		return index;
	}

	void FrameBuffer::AttachRenderBuffer(eTargetFormat format)
	{
		if (IsValid())
			return;

		RenderBuffer* target = new RenderBuffer();
		target->Create(m_Width, m_Height, format);

		m_RenderBuffers.push_back(target);
	}

	bool FrameBuffer::Create()
	{
		// Can't create buffer with no targets
		if (m_Textures.empty() && m_RenderBuffers.empty())
			return false;

		if (IsValid())
			return false;

		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		// Amount of color buffers we have
		std::vector<uint32> colorbuffers;
		int32 count = 0;

		for (uint32 i = 0; i < m_Textures.size(); ++i)
		{
			const TextureTarget& target = *m_Textures[i];

			// Attach this texture (record attachment point if color
			if (target.BoundToFrameBuffer(count))
				colorbuffers.push_back(GL_COLOR_ATTACHMENT0 + count++);
		}

		for (uint32 i = 0; i < m_RenderBuffers.size(); ++i)
		{
			const RenderBuffer& target = *m_RenderBuffers[i];

			// Attach this buffer (record attachment point if color
			if (target.BoundToFrameBuffer(count))
				colorbuffers.push_back(GL_COLOR_ATTACHMENT0 + count++);
		}

		// Set the drawing buffers
		glDrawBuffers(colorbuffers.size(), colorbuffers.data());

		// Assure we have set up correctly
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Error: Failed to create render target" << std::endl;

			// Forcefully destroy resource
			Clear();
			
			return false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		return true;
	}

	bool FrameBuffer::Destroy()
	{
		if (IsValid())
			Clear();

		return true;
	}

	void FrameBuffer::Blit(const FrameBuffer& read, const FrameBuffer& draw, eBufferBit buffers, eFilterMode filter)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, read.m_FBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, draw.m_FBO);

		glBlitFramebuffer(0, 0, read.m_Width, read.m_Height, 0, 0, draw.m_Width, draw.m_Height, (uint32)buffers, (uint32)filter);
	}

	void FrameBuffer::Clear()
	{
		glDeleteFramebuffers(1, &m_FBO);
		
		for (RenderTarget* target : m_Textures)
		{
			target->Destroy();
			delete target;
		}

		for (RenderTarget* target : m_RenderBuffers)
		{
			target->Destroy();
			delete target;
		}

		m_Textures.clear();
		m_RenderBuffers.clear();
	}
}