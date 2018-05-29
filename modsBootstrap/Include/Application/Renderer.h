#pragma once

#include "Rendering\Shaders\Shader.h"
#include "Rendering\Textures\Texture2D.h"

#include "Rendering\Lighting\LightRenderData.h"

#include "Buffers\CameraUniformBuffer.h"

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
	public:

		Renderer(int32 width, int32 height);
		Renderer(const Renderer& rhs) = delete;
		Renderer(Renderer&& rhs);

		virtual ~Renderer();

		Renderer& operator = (const Renderer& rhs) = delete;
		Renderer& operator = (Renderer&& rhs);

	public:

		// Draws a mesh
		void DrawMesh(const Mesh& mesh, const glm::mat4x4& transform);

		// Draws a model
		void DrawModel(const Model& model, const glm::mat4x4& transform);

	public:

		// Adds a light to render
		int32 AddLight(const Light& light);

		// Removes light at given index
		void RemoveLight(const Light& light, int32 index);

		// Updates the lights data at the given index
		bool UpdateLight(const Light& light, int32 index);

	public:

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

	private:

		// Adds a new directional light
		int32 AddDirectionalLight(const DirectionalLight& light);

		// Adds a new point light
		int32 AddPointLight(const PointLight& light);

		// Adds a new spot light
		int32 AddSpotLight(const SpotLight& light);

	protected:

		// Handle to the geometry frame buffer
		uint32 m_FBO;

		// Handle to the depth and stencil render buffer
		uint32 m_RBO;

		// Texture containing position data
		Texture2D m_PosBuffer;

		// Texture containing normals
		Texture2D m_NorBuffer;

		// Texture containing diffuse and specular
		Texture2D m_AlbBuffer;

	protected:

		// Shader for applying lighting
		ShaderProgram m_LShader;

		// Shader for post processing the frame
		ShaderProgram m_PShader;

	protected:

		// All directional lights in the scene
		std::vector<DirectionalLightData> m_DirLights;

		// Count of the amount of directional lights
		int32 m_DirCount;

		// All point lights in the scene
		//std::array<PointLightData, 10> m_PntLights;

		// Count of the amount of point lights
		int32 m_PntCount;

		// All spot lights in the scene
		//std::array<SpotLightData, 10> m_SptLights;

		// Count of the amount of spot lights
		int32 m_SptCount;
	};
}