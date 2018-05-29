#pragma once

#include "Rendering\Textures\Texture2D.h"

#include <vector>

namespace mods
{
	class RenderTarget
	{
	public:

		RenderTarget(uint32 width, uint32 height);
		RenderTarget(const RenderTarget& rhs) = delete;
		RenderTarget(RenderTarget&& rhs);

		virtual ~RenderTarget();

		RenderTarget& operator = (const RenderTarget& rhs) = delete;
		RenderTarget& operator = (RenderTarget&& rhs);

	public:

		// Binds this target for use
		void Bind();

		// Unbinds this target from use
		void Unbind();

	public:

		// Adds a new attachment to this buffer
		// Only valid if no handle has been generated
		int32 AttachTarget(eTextureFormat format);

		// Generates a new render target if buffers have been attached
		// Only valid if no handle has been generated
		virtual bool Create();

		// Destroys existing targets if any
		virtual bool Destroy();

	public:

		inline bool IsValid() const { return m_FBO != 0; }

		inline uint32 GetHandle() const { return m_FBO; }

		inline int32 GetWidth() const { return m_Width; }
		inline int32 GetHeight() const { return m_Height; }

		inline uint32 GetTargetCount() const { return m_Textures.size(); }

		// Get the target texture at the given index
		inline const Texture2D& GetTarget(int32 index) const { return m_Textures[index]; }

	private:

		// Clears all resources
		void Clear();

	protected:

		// Width and height of the frame buffer
		int32 m_Width, m_Height;

		// Handle to the frame buffer
		uint32 m_FBO;

		// All textures attached to this target
		std::vector<Texture2D> m_Textures;

		// Handle to the depth and stencil buffer
		uint32 m_RBO;
	};
}