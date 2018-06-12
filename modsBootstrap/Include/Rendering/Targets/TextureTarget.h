#pragma once

#include "RenderTarget.h"

namespace mods
{
	class TextureTarget : public RenderTarget
	{
	public:

		virtual ~TextureTarget() = default;

	public:

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