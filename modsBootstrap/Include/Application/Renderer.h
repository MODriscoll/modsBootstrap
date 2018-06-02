#pragma once

#include "Rendering\Shaders\Shader.h"
#include "Rendering\Textures\Texture2D.h"

#include "Rendering\Lighting\LightRenderData.h"

#include "Rendering\Targets\RenderTarget.h"

#include "Buffers\CameraUniformBuffer.h"
#include "Buffers\LightUniformBuffer.h"

#include <vector>

namespace mods
{
	class Camera;

	class Font;
	class Mesh;
	class Model;
	class Texture;

	class Light;

	class Material;

	// First iteration of renderer
	class Renderer
	{
		friend class Application;

	private:

		Renderer(uint32 width, uint32 height);
		virtual ~Renderer();

	public:

		// Set the uniform data for the camera
		// Should be called before frame starts
		static void SetCamera(const Camera& camera);

	public:

		// Draws a mesh
		static void DrawMesh(const Mesh& mesh, ShaderProgram& program, const glm::mat4x4& transform);

		// Draws a model
		static void DrawModel(const Model& model, ShaderProgram& program, const glm::mat4x4& transform);

	public:

		// Adds a light to render
		static int32 AddLight(const Light& light);

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

		// Initializes framebuffer and shaders
		virtual bool Initialize(int32 width, int32 height);

		// Cleans up resources
		virtual bool Cleanup();

	protected:

		// Singleton instance
		static Renderer* m_Singleton;

		static void Create(int32 width, int32 height);
		static void Destroy();

	protected:

		// Target to draw geometry phase to
		RenderTarget m_GTarget;

		// Index to various textures of the geometry target
		int32 m_PosIdx, m_NorIdx, m_AlbIdx;

	protected:

		// Shader for applying lighting
		ShaderProgram m_LShader;

	protected:

		// Camera uniform buffer
		CameraUniforms m_CameraUniform;

		// Lights uniform buffer
		LightUniforms m_LightUniform;

	private:

		// TODO: make vertex array object
		uint32 m_VAO, m_VBO, m_IBO;

		// Width and height of the viewport
		int32 m_Width, m_Height;
	};
}