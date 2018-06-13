#pragma once

#include "RenderBuffer.h"
#include "TextureTarget.h"

#include <vector>

#include "IncludeGLFW.h"

namespace mods
{
	enum class eBufferBit : uint32
	{
		Color		= 0x4000,
		Depth		= 0x0100,
		Stencil		= 0x0400
	};

	inline constexpr eBufferBit operator | (eBufferBit lhs, eBufferBit rhs)
	{
		return static_cast<eBufferBit>(static_cast<uint32>(lhs) | static_cast<uint32>(rhs));
	}

	class FrameBuffer
	{
	public:

		FrameBuffer(uint32 width, uint32 height);
		FrameBuffer(const FrameBuffer& rhs) = delete;
		FrameBuffer(FrameBuffer&& rhs);

		virtual ~FrameBuffer();

		FrameBuffer& operator = (const FrameBuffer& rhs) = delete;
		FrameBuffer& operator = (FrameBuffer&& rhs);

	public:

		// Binds this target for use
		void Bind();

		// Unbinds this target from use
		void Unbind();

	public:

		// Creates a new 2D texture render target
		// Index to texture is returned, index is invalid on failure
		int32 AttachTexture2D(eTargetFormat format, eFilterMode filter = eFilterMode::Nearest, eWrapMode wrap = eWrapMode::Edge);

		// Creates a new render buffer render target
		void AttachRenderBuffer(eTargetFormat format);

		// Generates the frame buffer if targets have been attached
		// Can only be used if IsValid returns false
		bool Create();

		// Destroys existing targets if any
		bool Destroy();

	public:

		// Performs a blit operation between two frame buffers
		static void Blit(const FrameBuffer& read, const FrameBuffer& draw, eBufferBit buffers, eFilterMode filter = eFilterMode::Nearest);

	public:

		// If this frame buffer is valid
		inline bool IsValid() const { return m_FBO != 0; }

		// Direct access to handle
		inline uint32 GetHandle() const { return m_FBO; }

		// Width and height of this frame buffer
		inline int32 GetWidth() const { return m_Width; }
		inline int32 GetHeight() const { return m_Height; }

		// Total amount of targets attached to this frame buffer
		inline uint32 GetTargetCount() const { return m_Textures.size() + m_RenderBuffers.size(); }

		// Amount of textures attached to this frame buffer
		inline uint32 GetTextureCount() const { return m_Textures.size(); }

		// Get the target texture at the given index
		inline const TextureTarget& GetTarget(int32 index) const { return *m_Textures[index]; }

		// Binds target at the given index to given slot
		inline void BindTarget(int32 index, uint32 slot = 0) const { m_Textures[index]->Bind(slot); }

	private:

		// Clears all resources
		void Clear();

	protected:

		// Width and height of the frame buffer
		int32 m_Width, m_Height;

		// Handle to the frame buffer
		uint32 m_FBO;

		// All textures attached to this target
		std::vector<TextureTarget*> m_Textures;

		// All render buffers attached to this buffer
		std::vector<RenderBuffer*> m_RenderBuffers;
	};
}