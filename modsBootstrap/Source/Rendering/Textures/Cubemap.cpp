#include "Rendering\Textures\Cubemap.h"

#include "IncludeGLFW.h"
#include "Rendering\Textures\TextureUtility.h"

#include <iostream>

// TODO: move elsewhere, this is for reference
// cube map textures order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)

namespace mods
{
	Cubemap::Cubemap(const std::array<std::string, 6>& paths)
		: m_Handle(0)
	{
		Load(paths);
	}

	Cubemap::Cubemap(Cubemap&& rhs)
		: m_Handle(rhs.m_Handle)
	{
		m_Handle = rhs.m_Handle;

		rhs.m_Handle = 0;
	}

	Cubemap::~Cubemap()
	{
		Unload();
	}

	Cubemap& Cubemap::operator=(Cubemap&& rhs)
	{
		if (IsValid())
			Unload();

		m_Handle = rhs.m_Handle;

		rhs.m_Handle = 0;

		return *this;
	}

	bool Cubemap::Load(const std::array<std::string, 6>& paths)
	{
		if (IsValid())
			Unload();

		glGenTextures(1, &m_Handle);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_Handle);

		// TODO: remove
		detail::SetFlip(false);

		// Load each texture from their respective path
		detail::TextureData data;
		for (uint32 i = 0; i < 6; ++i)
		{
			if (detail::LoadTextureFromSource(paths[i], data))
			{
				// Generate the texture
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, data.InternalFormat, 
					data.Width, data.Height, 0, data.Format, GL_UNSIGNED_BYTE, data.Pixels);
				detail::DestroyTexture(data.Pixels);
			}
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		return true;
	}

	bool Cubemap::Unload()
	{
		if (!m_Handle)
			return true;

		glDeleteTextures(1, &m_Handle);

		m_Handle = 0;

		return true;
	}

	void Cubemap::Bind(uint32 slot)
	{
		#if _DEBUG
		if (!IsValid())
			std::cout << "Warning: Cubemap bound to slot " << slot << " is not valid" << std::endl;
		#endif

		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_Handle);
	}

	void Cubemap::Unbind()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glActiveTexture(GL_TEXTURE0);
	}
}