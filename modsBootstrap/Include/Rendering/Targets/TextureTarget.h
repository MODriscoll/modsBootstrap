#pragma once

#include "RenderTarget.h"

namespace mods
{
	enum class eWrapMode : int32
	{
		Repeat		= 0x2901,
		Mirror		= 0x8370,
		Edge		= 0x812F,
		Border		= 0x812D
	};

	enum class eFilterMode : int32
	{
		Nearest		= 0x2600,
		Linear		= 0x2601

		// TODO: mipmap
	};

	class TextureTarget : public RenderTarget
	{
	public:

		virtual ~TextureTarget() = default;

	public:

		// Creates new target with given type and format
		virtual bool Create(int32 width, int32 height, eTargetFormat format, eFilterMode filter, eWrapMode wrap) = 0;

		// Destroys existing texture if any
		virtual bool Destroy() override;

		// Binds this target for use at given slot
		void Bind(uint32 slot = 0) const;

		// Unbinds this target from use
		void Unbind() const;

	protected:

		// Get the pixel format based on an internal format
		uint32 GetPixelFormat(eTargetFormat intern) const;

		// Get data type needed for an internal format
		uint32 GetDataType(eTargetFormat intern) const;

	protected:

		virtual uint32 GetTextureType() const = 0;
	};
}