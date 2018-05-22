#pragma once

#include "Buffer.h"

#include <vector>

namespace mods
{
	struct FrameBuffer
	{
	public:

		FrameBuffer(int32 width, int32 height);
		FrameBuffer(const FrameBuffer& rhs) = delete;
		FrameBuffer(FrameBuffer&& rhs);

		virtual ~FrameBuffer();

		FrameBuffer& operator = (const FrameBuffer& rhs) = delete;
		FrameBuffer& operator = (FrameBuffer&& rhs);

	public:

		inline bool IsValid() const { return m_FBO != 0; }

	protected:

		// Returns a readable string for the given error
		constexpr const char* GetStringForError(uint32 error) const;

	protected:

		// Handle to the buffer
		uint32 m_FBO;
	};
}