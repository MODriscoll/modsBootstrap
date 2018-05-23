#pragma once

#include "Buffers\FrameBuffer.h"
#include "Buffers\UniformBuffer.h"

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
		None					= 0,
		GeometryPass			= 1 << 0,
		LightingPass			= 1 << 1,
		PostProcessPass			= 1 << 2
	};

	// Uniforms relating to the camera
	struct CameraUniform : public UniformBuffer
	{
	public:

		CameraUniform();

	public:

		// Sets uniform variables for the camera
		void SetUniforms(const Camera& camera);
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

		// Called to prepare for the next frame
		void StartFrame();

		// Called to prepare the geometry pass 
		void StartGeometryPass();

		// Called to start lighting pass
		void StartLightingPass();

		// Called to start post process pass
		void StartPostProcessPass();

		// Called to end the current frame
		void EndFrame();

	public:


	protected:

		// Geometry step frame buffer
		FrameBuffer m_GBuffer;

	private:

		// What stage of the render pass we are at
		eRenderStage m_Stage;
	};
}