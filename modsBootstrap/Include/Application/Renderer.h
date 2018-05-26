#pragma once

#include "Buffers\FrameBuffer.h"
#include "Buffers\UniformBuffer.h"
#include "Rendering\Shaders\Shader.h"

namespace mods
{
	class Camera;

	class Font;
	class Mesh;
	class Model;
	class Texture;

	class Light;

	class Material;

	enum eRenderStage : byte
	{
		None					= 0,
		GeometryPass			= 1 << 0,
		LightingPass			= 1 << 1,
		PostProcessPass			= 1 << 2
	};

	// First iteration of renderer
	class Renderer
	{
		friend class Application;

	public:

		// Draws a mesh
		virtual void DrawMesh(const Mesh& mesh, const glm::mat4x4& transform);

		// Draws a model
		virtual void DrawModel(const Model& model, const glm::mat4x4& transform);

	public:

		// Adds a light to render
		virtual void AddLight(const Light& light);

	private:

		Renderer();
		virtual ~Renderer();

		// Initializes this renderer
		virtual void Initialize(int32 width, int32 height);

		// Cleans up this renderer
		virtual void Cleanup();

	protected:

		// Starts new frame of rendering
		virtual void StartFrame();

		// Called to start the geometry pass
		virtual void StartGeometryPass();

		// Called to start the lighting pass
		virtual void StartLightingPass();

		// Called to start the post processing pass
		virtual void StartPostProcessPass();

		// Ends the frame of rendering
		virtual void EndFrame();

	protected:

		// Geometry step frame buffer
		FrameBuffer m_GBuffer;

		// Shader for drawing geometry
		ShaderProgram m_GShader;

		// Shader for applying lighting
		ShaderProgram m_LShader;

		// Shader for post processing the frame
		ShaderProgram m_PShader;

	private:

		// All lights to draw
		std::vector<const Light*> m_Lights;

		// What stage of the render pass we are at
		eRenderStage m_Stage;
	};
}