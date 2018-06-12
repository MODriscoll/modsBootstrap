#include "Rendering\Targets\RenderTarget.h"

#include <cassert>

namespace mods
{
	RenderTarget::RenderTarget()
		: m_Handle(0)
		, m_Format(eTargetFormat::R)
	{

	}

	RenderTarget::RenderTarget(RenderTarget&& rhs)
		: m_Handle(rhs.m_Handle)
		, m_Format(rhs.m_Format)
	{
		rhs.m_Handle = 0;
		rhs.m_Format = eTargetFormat::R;
	}

	RenderTarget::~RenderTarget()
	{
		// Causing unresolved external symbol for some reason
		//Destroy();
	}

	RenderTarget& RenderTarget::operator=(RenderTarget&& rhs)
	{
		// Destroy potential target
		Destroy();

		m_Handle = rhs.m_Handle;
		m_Format = rhs.m_Format;

		rhs.m_Handle = 0;
		rhs.m_Format = eTargetFormat::R;

		return *this;
	}

	uint32 RenderTarget::GetAttachmentType(int32 index, bool& iscolor) const
	{
		iscolor = false;

		switch (m_Format)
		{
			case eTargetFormat::R:
			case eTargetFormat::RG:
			case eTargetFormat::RGB:
			case eTargetFormat::RGBA:
			case eTargetFormat::R16F:
			case eTargetFormat::RG16F:
			case eTargetFormat::RGB16F:
			case eTargetFormat::RGBA16F:
			case eTargetFormat::R32F:
			case eTargetFormat::RG32F:
			case eTargetFormat::RGB32F:
			case eTargetFormat::RGBA32F:
			{
				iscolor = true;
				return GL_COLOR_ATTACHMENT0 + index;
			}

			case eTargetFormat::Depth:
			case eTargetFormat::Depth16:
			case eTargetFormat::Depth24:
			case eTargetFormat::Depth32:
			{
				return GL_DEPTH_ATTACHMENT;
			}

			case eTargetFormat::Depth24Stencil8:
			{
				return GL_DEPTH_STENCIL_ATTACHMENT;
			}

			default:
			{
				assert(false);
			}
		}

		return GL_COLOR_ATTACHMENT0;
	}
}