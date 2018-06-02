#include "Rendering\Targets\RenderTarget.h"

#include "IncludeGLFW.h"

#include <iostream>

namespace mods
{
	RenderTarget::RenderTarget(uint32 width, uint32 height)
		: m_FBO(0)
		, m_Width(width)
		, m_Height(height)
	{

	}

	RenderTarget::RenderTarget(RenderTarget&& rhs)
		: m_FBO(rhs.m_FBO)
		, m_Width(rhs.m_Width)
		, m_Height(rhs.m_Height)
		, m_Textures(std::move(rhs.m_Textures))
		, m_RBO(rhs.m_RBO)
	{
		rhs.m_FBO = 0;
		rhs.m_Width = 0;
		rhs.m_Height = 0;
		rhs.m_RBO = 0;
	}

	RenderTarget::~RenderTarget()
	{
		Destroy();
	}

	RenderTarget& RenderTarget::operator=(RenderTarget&& rhs)
	{
		// Destroy potential existing buffer
		Destroy();

		m_FBO = rhs.m_FBO;
		m_Width = rhs.m_Width;
		m_Height = rhs.m_Height;
		m_Textures = std::move(rhs.m_Textures);
		m_RBO = rhs.m_RBO;

		rhs.m_FBO = 0;
		rhs.m_Width = 0;
		rhs.m_Height = 0;
		rhs.m_RBO = 0;

		return *this;
	}

	void RenderTarget::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	}

	void RenderTarget::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	int32 RenderTarget::AttachTarget(eTextureFormat format)
	{
		if (IsValid())
			return -1;

		Texture2D target;
		target.Create(m_Width, m_Height, format);

		// Capture size before returning, so we
		// have the index of the last texture
		int32 index = m_Textures.size();
		m_Textures.push_back(std::move(target));
		return index;
	}

	bool RenderTarget::Create()
	{
		// Can't create buffer with no targets
		if (m_Textures.empty())
			return false;

		if (IsValid())
			return false;

		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		// Reserve what we need
		std::vector<uint32> attachments;
		attachments.reserve(m_Textures.size());

		for (uint32 i = 0; i < m_Textures.size(); ++i)
		{
			const Texture& target = m_Textures[i];
			uint32 index = GL_COLOR_ATTACHMENT0 + i;

			// Attach the texture
			glFramebufferTexture2D(GL_FRAMEBUFFER, index, target.GetTextureType(), target.GetHandle(), 0);

			// Add this color as being utilized
			attachments.push_back(index);
		}

		// Inform which color attachments we are using
		glDrawBuffers(attachments.size(), attachments.data());

		// Generate the depth and stencil buffer
		// TODO: add ability to customize
		glGenRenderbuffers(1, &m_RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);

		// Create the depth and stencil buffer
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);

		// Attach the render buffer to the frame buffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

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

	bool RenderTarget::Destroy()
	{
		if (IsValid())
			Clear();

		return true;
	}

	void RenderTarget::Clear()
	{
		glDeleteFramebuffers(1, &m_FBO);
		glDeleteRenderbuffers(1, &m_RBO);

		m_Textures.clear();
	}
}