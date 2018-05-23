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
	{
		Load(paths);
	}

	Cubemap::Cubemap(Cubemap&& rhs)
		: Texture(std::move(rhs))
	{

	}

	Cubemap& Cubemap::operator=(Cubemap&& rhs)
	{
		Texture::operator=(std::move(rhs));

		return *this;
	}

	bool Cubemap::Load(const std::array<std::string, 6>& paths)
	{
		detail::SetFlip(false);

		// Report if at least one texture failed to load
		bool bFail = false;

		// Destroy potential existing texture
		Destroy();

		glGenTextures(1, &m_Handle);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_Handle);

		// Load each texture from their respective path
		detail::TextureData data;
		for (uint32 i = 0; i < 6; ++i)
		{
			if (!detail::LoadTextureFromSource(paths[i], data))
			{
				std::cout << "Error: Failed to load cubemap texture " << i << " - Path=" << paths[i] << std::endl;
				bFail = true;
				continue;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, data.InternalFormat, data.Width, data.Height, 0, data.Format, GL_UNSIGNED_BYTE, data.Pixels);

			// No longer need the pixels
			detail::DestroyTexture(data.Pixels);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		return bFail;
	}

	uint32 Cubemap::GetTextureType() const
	{
		return GL_TEXTURE_CUBE_MAP;
	}
}