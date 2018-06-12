#pragma once

#include "RenderBuffer.h"
#include "TextureTarget.h"

#include <vector>

namespace mods
{
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

		// Adds a new texture to the frame buffer
		// Index will be invalid if either the frame buffer has
		// already been created or type is a render buffer
		bool AttachTarget(eTargetType type, eTargetFormat format, int32* index = nullptr);

		// Generates the frame buffer if targets have been attached
		// Can only be used if IsValid returns false
		virtual bool Create();

		// Destroys existing targets if any
		virtual bool Destroy();

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