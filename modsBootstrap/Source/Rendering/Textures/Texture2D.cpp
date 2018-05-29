#include "Rendering\Textures\Texture2D.h"

#include "IncludeGLFW.h"
#include "Rendering\Textures\TextureUtility.h"

#include <iostream>

namespace mods
{
	Texture2D::Texture2D()
		: m_Width(0)
		, m_Height(0)
		, m_Channels(eTextureChannels::Unknown)
	{

	}

	Texture2D::Texture2D(const std::string& path, eTextureChannels channels, bool sRGB)
		: m_Width(0)
		, m_Height(0)
		, m_Channels(eTextureChannels::Unknown)
	{
		Load(path, channels, sRGB);
	}

	Texture2D::Texture2D(Texture2D&& rhs)
		: Texture(std::move(rhs))
		, m_Width(rhs.m_Width)
		, m_Height(rhs.m_Height)
		, m_Channels(rhs.m_Channels)
	{
		rhs.m_Width = 0;
		rhs.m_Height = 0;
		rhs.m_Channels = eTextureChannels::Unknown;
	}

	Texture2D& Texture2D::operator=(Texture2D&& rhs)
	{
		Texture::operator=(std::move(rhs));

		m_Width = rhs.m_Width;
		m_Height = rhs.m_Height;
		m_Channels = rhs.m_Channels;

		rhs.m_Width = 0;
		rhs.m_Height = 0;
		rhs.m_Channels = eTextureChannels::Unknown;

		return *this;
	}

	bool Texture2D::Load(const std::string& path, eTextureChannels channels, bool sRGB)
	{
		detail::SetFlip(true);

		detail::TextureData data;
		if (!detail::LoadTextureFromSource(path, data, channels, sRGB))
		{
			std::cout << "Error: Failed to load texture - Path=" << path << std::endl;
			return false;
		}

		// Destroy potential existing texture
		Destroy();

		glGenTextures(1, &m_Handle);
		glBindTexture(GL_TEXTURE_2D, m_Handle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		glTexImage2D(GL_TEXTURE_2D, 0, data.InternalFormat, data.Width, data.Height, 0, data.Format, GL_UNSIGNED_BYTE, data.Pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
		
		// No longer need the pixels
		detail::DestroyTexture(data.Pixels);

		m_Width = data.Width;
		m_Height = data.Height;
		m_Channels = (eTextureChannels)data.Channels;

		return true;
	}

	bool Texture2D::Create(int32 width, int32 height, eTextureFormat format, byte* pixels)
	{
		uint32 form = detail::GetDataFormat(format);
		int32 intern = (int32)format;

		// Destroy potential existing texture
		Destroy();

		glGenTextures(1, &m_Handle);
		glBindTexture(GL_TEXTURE_2D, m_Handle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		glTexImage2D(GL_TEXTURE_2D, 0, intern, width, height, 0, form, GL_UNSIGNED_BYTE, pixels);

		glBindTexture(GL_TEXTURE_2D, 0);

		m_Width = width;
		m_Height = height;
		m_Channels = detail::GetTextureChannels(form);

		return true;
	}

	uint32 Texture2D::GetTextureType() const
	{
		return GL_TEXTURE_2D;
	}
}
