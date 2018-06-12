#pragma once

#include "RenderTarget.h"

namespace mods
{
	class RenderBuffer : public RenderTarget
	{
	public:

		virtual ~RenderBuffer() = default;

	public:

		// Creates a new render buffer with given format
		virtual bool Create(int32 width, int32 height, eTargetFormat format) override;

		// Destroys existing buffer if any
		virtual bool Destroy() override;

		// Bounds this target as a render buffer
		virtual bool BoundToFrameBuffer(int32 index) const override;

	public:

		// Get this targets type
		virtual eTargetType GetTargetType() const override { return eTargetType::RenderBuffer; }
	};
}