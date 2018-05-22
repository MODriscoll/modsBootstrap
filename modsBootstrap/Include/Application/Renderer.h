#pragma once

#include "Types.h"

namespace mods
{
	class Camera;

	class ShaderProgram;

	class Font;
	class Mesh;
	class Model;
	class Texture;

	class Material;

	enum eRenderStage : byte
	{
		GeometryPass			= 1 << 0,
		LightingPass			= 1 << 1,
		PostProcessPass			= 1 << 2
	};

	// First iteration of renderer
	class Renderer
	{
	public:

		Renderer();
		virtual ~Renderer();

	public:

		// Initializes this renderer
		void Initialize(int32 width, int32 height);

		// Cleans up this renderer
		void Cleanup();

	public:

		// Called to prepare the geometry pass 
		void StartGeometryPass();

		// Called to start lighting pass
		void StartLightingPass();


	public:




	protected:

		// Handle to the gbuffer
		// Is our deffered frame buffer to render to
		uint32 m_GBuffer;

		// Indivual buffers to draw to
		// Are textures, so we can read from them
		uint32 m_GPosition, m_GNormal, m_GAlbedo;

		// Handle to the depth/stencil
		// Is a render buffer, so we can't read from it
		// Records 24 bits of depth, 8 bits of stencil
		uint32 m_DSBuffer;

	private:

		// What stage of the render pass we are at
	};
}