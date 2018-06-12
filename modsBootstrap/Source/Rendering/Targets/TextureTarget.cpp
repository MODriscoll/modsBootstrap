#include "Rendering\Targets\TextureTarget.h"

#include "IncludeGLFW.h"

#include <cassert>

namespace mods
{
	bool TextureTarget::Destroy()
	{
		if (IsValid())
			glDeleteTextures(1, &m_Handle);

		m_Handle = 0;
		m_Format = eTargetFormat::R;

		return true;
	}

	void TextureTarget::Bind(uint32 slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GetTextureType(), m_Handle);
	}

	void TextureTarget::Unbind() const
	{
		glBindTexture(GetTextureType(), 0);
	}

	uint32 TextureTarget::GetPixelFormat(eTargetFormat intern) const
	{
		switch (intern)
		{
			case eTargetFormat::R:
			case eTargetFormat::R16F:
			case eTargetFormat::R32F:
			{
				return GL_RED;
			}

			case eTargetFormat::RG:
			case eTargetFormat::RG16F:
			case eTargetFormat::RG32F:
			{
				return GL_RG;
			}

			case eTargetFormat::RGB:
			case eTargetFormat::RGB16F:
			case eTargetFormat::RGB32F:
			{
				return GL_RGB;
			}

			case eTargetFormat::RGBA:
			case eTargetFormat::RGBA16F:
			case eTargetFormat::RGBA32F:
			{
				return GL_RGBA;
			}

			case eTargetFormat::Depth:
			case eTargetFormat::Depth16:
			case eTargetFormat::Depth24:
			case eTargetFormat::Depth32:
			{
				return GL_DEPTH_COMPONENT;
			}

			case eTargetFormat::Depth24Stencil8:
			{
				return GL_DEPTH_STENCIL;
			}

			default:
			{
				assert(false);
			}
		}

		return GL_RED;
	}

	uint32 TextureTarget::GetDataType(eTargetFormat intern) const
	{
		switch (intern)
		{
			case eTargetFormat::R:
			case eTargetFormat::RG:
			case eTargetFormat::RGB:
			case eTargetFormat::RGBA:
			{
				return GL_UNSIGNED_BYTE;
			}

			case eTargetFormat::R16F:
			case eTargetFormat::RG16F:
			case eTargetFormat::RGB16F:
			case eTargetFormat::RGBA16F:
			{
				return GL_HALF_FLOAT;
			}

			case eTargetFormat::R32F:
			case eTargetFormat::RG32F:
			case eTargetFormat::RGB32F:
			case eTargetFormat::RGBA32F:
			{
				return GL_FLOAT;
			}

			case eTargetFormat::Depth:
			case eTargetFormat::Depth16:
			case eTargetFormat::Depth24:
			case eTargetFormat::Depth32:
			{
				return GL_FLOAT;
			}

			case eTargetFormat::Depth24Stencil8:
			{
				return GL_UNSIGNED_INT_24_8;
			}

			default:
			{
				assert(false);
			}
		}

		return GL_UNSIGNED_BYTE;
	}
}