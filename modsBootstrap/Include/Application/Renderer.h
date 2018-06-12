#pragma once

#include "Rendering\Shaders\Shader.h"
#include "Rendering\Textures\Texture2D.h"

#include "Rendering\Lighting\LightRenderData.h"

#include "Rendering\Targets\FrameBuffer.h"

#include "Buffers\CameraUniformBuffer.h"
#include "Buffers\LightUniformBuffer.h"
#include "Buffers\AppUniformBuffer.h"

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

		// Set the camera to use to render the scene
		static void SetCamera(const Camera& camera);

	public:

		// Draws a mesh
		static void DrawMesh(const Mesh& mesh, ShaderProgram& program, const glm::mat4x4& transform);

		// Draws a model
		static void DrawModel(const Model& model, ShaderProgram& program, const glm::mat4x4& transform);

	public:

		// Adds a light to render
		static int32 AddLight(const Light& light);

		// Updates a light being rendered
		static void UpdateLight(const Light& light, int32 index);

	public:

		// Enables/Disables wireframe rendering
		static void EnableWireframe(bool enable);

	public:

		// Enables/Disables gamma correction
		static void EnableGammaCorrection(bool enable);

		// Sets the exponent to use for gamma correction
		static void SetGammaExponent(float gamma);

		// Sets the exposure for tone mapping HDR lighting
		static void SetExposure(float exposure);

	protected:

		// Starts new frame of rendering
		virtual void StartFrame(float time);

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

	private:

		// Applies bloom to the scene
		void ApplyBloom();

	private:

		void GenerateSphere();

	protected:

		// If we are rendering in wireframe
		bool m_bRenderWireframe;

	protected:

		// If gamma correction is activated
		bool m_bGammaCorrect;

		// Gamma exponent for gamma correction
		float m_GammaExponent;

		// Exposure for tonemapping high dynamic range lighting
		float m_HDRExposure;

	protected:

		// Buffer to draw geometry phase to
		FrameBuffer m_GBuffer;

		// Index of position texture
		int32 m_PosIdx;
		// Index of normal texture
		int32 m_NorIdx;
		// Index of albedo and specular texture
		int32 m_AlbIdx;

		// Buffer to draw lighting phase to
		FrameBuffer m_LBuffer;

		// Index to lighting texture
		int32 m_ColIdx;

		// Buffer to draw post processing to
		FrameBuffer m_PBuffer;

		// Index to albedo + lighting scene pre-post processing (no post applied)
		int32 m_NoPostIdx;
		// Index to scene with color values greate than brightness threshold
		int32 m_BrightIdx;

		// Ping, pong buffer used to draw bloom effect
		FrameBuffer m_PingBuffer, m_PongBuffer;

	protected:

		// The camera to use when rendering
		const Camera* m_Camera;

	protected:

		// Shader for performing stencil test
		ShaderProgram m_SShader;

		// Shaders for applying compisition
		ShaderProgram m_DIRShader;
		ShaderProgram m_PNTShader;

		// Shader for applying composition
		ShaderProgram m_CShader;

		// Shader for applying initial post processing
		ShaderProgram m_IPShader;

		// Shader for applying bloom effect
		ShaderProgram m_BloomShader;

		// Shader for applying finishing post processing
		ShaderProgram m_FPShader;

	protected:

		// Camera uniform buffer
		CameraUniforms m_CameraUniform;

		// Lights uniform buffer
		LightUniforms m_LightUniform;

		// Application uniform buffer
		AppUniforms m_AppUniform;

	private:

		// TODO: make vertex array object
		uint32 m_VAO, m_VBO, m_IBO;
		uint32 m_sphereVAO, m_sphereVBO, m_sphereIBO;
		uint32 m_sphereIndices;

		// Width and height of the viewport
		int32 m_Width, m_Height;
	};
}