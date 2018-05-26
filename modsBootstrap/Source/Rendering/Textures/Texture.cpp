#include "Rendering\Textures\Texture.h"

#include "IncludeGLFW.h"
#include "Rendering\Textures\TextureUtility.h"

#include <cassert>
#include <iostream>

namespace mods
{
	Texture::Texture()
		: m_Handle(0)
	{

	}

	Texture::Texture(Texture&& rhs)
		: m_Handle(rhs.m_Handle)
	{
		rhs.m_Handle = 0;
	}

	Texture::~Texture()
	{
		Destroy();
	}

	Texture& Texture::operator=(Texture&& rhs)
	{
		// Destroy potential existing texture
		Destroy();

		m_Handle = rhs.m_Handle;

		rhs.m_Handle = 0;

		return *this;
	}

	bool Texture::Destroy()
	{
		if (IsValid())
			glDeleteTextures(1, &m_Handle);

		return true;
	}

	void Texture::Bind(uint32 slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GetTextureType(), m_Handle);
	}

	void Texture::Unbind() const
	{
		glBindTexture(GetTextureType(), 0);
	}
}