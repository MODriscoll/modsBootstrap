#pragma once

#include "TextureTarget.h"

namespace mods
{
	class Texture2DTarget : public TextureTarget
	{
	public:

		virtual ~Texture2DTarget() = default;

	public:

		// Creates a 2D texture with given format
		virtual bool Create(int32 width, int32 height, eTargetFormat format) override;

		// Bounds this target as a texture 2D
		virtual bool BoundToFrameBuffer(int32 index) const override;

	public:

		// Get this targets type
		virtual eTargetType GetTargetType() const override { return eTargetType::Texture2D; }

	protected:

		virtual uint32 GetTextureType() const override;
	};
}