#include "Rendering\Textures\Texture.h"

#include "IncludeGLFW.h"

#include <stb\stb_image.h>

#include <iostream>

namespace mods
{
	Texture::Texture(const std::string& path)
		: m_Handle(0)
		, m_Pixels(nullptr)
		, m_Width(0)
		, m_Height(0)
		, m_Channels(0)
	{
		Load(path);
	}

	Texture::Texture(Texture&& rhs)
	{
		if (IsValid())
			Unload();

		// Extract contents from other texture
		m_Handle = rhs.m_Handle;
		m_Pixels = rhs.m_Pixels;
		m_Width = rhs.m_Width;
		m_Height = rhs.m_Height;
		m_Channels = rhs.m_Channels;

		// Nullify other textures references
		rhs.m_Handle = 0;
		rhs.m_Pixels = nullptr;
		rhs.m_Width = 0;
		rhs.m_Height = 0;
		rhs.m_Channels = 0;
	}

	Texture::~Texture()
	{
		Unload();
	}

	Texture& Texture::operator=(Texture&& rhs)
	{
		if (IsValid())
			Unload();

		// Extract contents from other texture
		m_Handle = rhs.m_Handle;
		m_Pixels = rhs.m_Pixels;
		m_Width = rhs.m_Width;
		m_Height = rhs.m_Height;
		m_Channels = rhs.m_Channels;

		// Nullify other textures references
		rhs.m_Handle = 0;
		rhs.m_Pixels = nullptr;
		rhs.m_Width = 0;
		rhs.m_Height = 0;
		rhs.m_Channels = 0;

		return *this;
	}

	bool Texture::Load(const std::string& path)
	{
		// TODO: add texture load options
		// or have debug texture class that saves out
		// texture and options to a unique file in which to load

		// TODO: optional
		stbi_set_flip_vertically_on_load(1);

		// Load the image from the file
		int width, height, channels;
		unsigned char* pixels = stbi_load(path.c_str(), &width, &height, &channels, STBI_default);

		// Abort if we failed to load texture
		if (!pixels)
		{
			std::cout << "Error: Failed to open texture source at path: " << path.c_str() << std::endl;
			stbi_image_free(pixels);
			return false;
		}

		glGenTextures(1, &m_Handle);
		glBindTexture(GL_TEXTURE_2D, m_Handle);

		// TODO: Texture options
		// Filtering options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// Wrap options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// TODO: get internal format and format (look into glTexImage2D)
		// Generate the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		m_Pixels = pixels;
		m_Width = width;
		m_Height = height;
		m_Channels = channels;

		// TODO: options to keep pixels, for now
		{
			stbi_image_free(m_Pixels);
			m_Pixels = nullptr;
		}

		return true;
	}

	bool Texture::Unload()
	{
		// Pixels will only be valid if handle is valid
		if (!m_Handle)
			return true;

		glDeleteTextures(1, &m_Handle);

		// Destroy local pixels if existing
		if (m_Pixels)
			stbi_image_free(m_Pixels);

		m_Handle = 0;
		m_Pixels = nullptr;
		m_Width = 0;
		m_Height = 0;
		m_Channels = 0;

		return true;
	}

	void Texture::Bind(unsigned int slot)
	{
	#if _DEBUG
		if (!IsValid())
			std::cout << "Warning: Texture bound to slot " << slot << " is not valid." << std::endl;
	#endif

		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_Handle);
	}

	void Texture::Unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}