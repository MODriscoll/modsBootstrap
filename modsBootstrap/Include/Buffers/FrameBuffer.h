#pragma once

#include "Buffer.h"
#include "Rendering\Textures\Texture2D.h"

#include <vector>

namespace mods
{
	enum eTextureBufferTypes : uint32
	{
		R		= 1,
		RG		= 2,
		RGB		= 3,
		RGBA	= 4
	};

	// TODO: make customizable
	// re-iterate
	struct FrameBuffer
	{
	public:

		FrameBuffer();
		FrameBuffer(const FrameBuffer& rhs) = delete;
		FrameBuffer(FrameBuffer&& rhs);

		virtual ~FrameBuffer();

		FrameBuffer& operator = (const FrameBuffer& rhs) = delete;
		FrameBuffer& operator = (FrameBuffer&& rhs);

	public:

		// Creates a new frame buffer with the given types of textures
		// A render buffer with depth and stencil is also created
		virtual bool Create(int32 width, int32 height, std::vector<eTextureBufferTypes> types);

	public:

		// Binds this framebuffer for use
		void Bind();

		// Unbinds this framebuffer
		void Unbind();

	public:

		inline bool IsValid() const { return m_FBO != 0; }

		// Only here to use glBlitBuffer
		// Make function that handles this internally
		inline int32 GetHandle() const { return m_FBO; }

		inline const Texture2D& GetTexture(int32 index) const { return m_Textures[index]; }

	protected:

		// Returns a readable string for the given error
		constexpr const char* GetStringForError(uint32 error) const;

	protected:

		// Handle to the buffer
		uint32 m_FBO;

		// Handle to the render buffer
		uint32 m_RBO;

		// All textures attached to this buffer
		// TODO: make a texturebuffer class instead (inherits from texture base)
		std::vector<Texture2D> m_Textures;
	};
}