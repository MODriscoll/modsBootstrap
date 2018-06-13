#include "Application\Renderer.h"

#include "IncludeGLFW.h"
#include "Rendering\Lighting\DirectionalLight.h"
#include "Rendering\Lighting\SpotLight.h"

#include "Rendering\Meshes\Model.h"

#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <cassert>
#include <iostream>

namespace mods
{
	Renderer* Renderer::m_Singleton = nullptr;

	Renderer::Renderer(uint32 width, uint32 height)
		: m_bRenderWireframe(false)
		, m_bGammaCorrect(true)
		, m_GammaExponent(2.2f)
		, m_HDRExposure(1.f)
		, m_bBloom(false)
		, m_BloomThreshold(1.f)
		, m_Camera(nullptr)
		, m_GBuffer(width, height)
		, m_LBuffer(width, height)
		, m_PBuffer(width, height)
		, m_PingBuffer(width, height)
		, m_PongBuffer(width, height)
		, m_Width(width)
		, m_Height(height)
	{
		
	}

	Renderer::~Renderer()
	{
		Cleanup();
	}

	void Renderer::SetCamera(const Camera& camera)
	{
		m_Singleton->m_Camera = &camera;
	}

	void Renderer::DrawMesh(const Mesh& mesh, ShaderProgram& program, const glm::mat4x4& transform)
	{
		program.Bind();

		glm::mat3 normal = glm::inverseTranspose(transform);
		program.SetUniformValue("model", transform);
		program.SetUniformValue("normat", normal);

		mesh.Draw(program);
	}

	void Renderer::DrawModel(const Model& model, ShaderProgram& program, const glm::mat4x4& transform)
	{
		program.Bind();

		glm::mat3 normal = glm::inverseTranspose(transform);
		program.SetUniformValue("model", transform);
		program.SetUniformValue("normat", normal);

		model.Draw(program);
	}

	int32 Renderer::AddLight(const Light& light)
	{
		switch (light.GetLightType())
		{
			case eLightType::Directional:
			{
				const DirectionalLight& dirlight = static_cast<const DirectionalLight&>(light);
				return m_Singleton->m_LightUniform.AddDirectionalLight(DirectionalLightData(dirlight));
			}

			case eLightType::Point:
			{
				const PointLight& pntlight = static_cast<const PointLight&>(light);
				return m_Singleton->m_LightUniform.AddPointLight(PointLightData(pntlight));
			}

			case eLightType::Spot:
			{
				//const SpotLight& sptlight = static_cast<const SpotLight&>(light);
				//return m_Singleton->m_LightUniform.AddSpotLight(SpotLightData(sptlight));
				return -1;
			}

			default:
			{
				assert(false);
			}
		}

		return -1;
	}

	void Renderer::UpdateLight(const Light& light, int32 index)
	{
		switch (light.GetLightType())
		{
			case eLightType::Directional:
			{
				const DirectionalLight& dirlight = static_cast<const DirectionalLight&>(light);
				m_Singleton->m_LightUniform.UpdateDirectionalLight(DirectionalLightData(dirlight), index);
				break;
			}

			case eLightType::Point:
			{
				const PointLight& dirlight = static_cast<const PointLight&>(light);
				m_Singleton->m_LightUniform.UpdatePointLight(PointLightData(dirlight), index);
				break;
			}

			case eLightType::Spot:
			{
				//const SpotLight& sptlight = static_cast<const SpotLight&>(light);
				//m_Singleton->m_LightUniform.UpdateSpotLight(SpotLightData(sptlight), index);
				break;
			}

			default:
			{
				assert(false);
			}
		}
	}

	void Renderer::EnableWireframe(bool enable)
	{
		m_Singleton->m_bRenderWireframe = enable;
	}

	void Renderer::EnableGammaCorrection(bool enable)
	{
		m_Singleton->m_bGammaCorrect = enable;
	}

	void Renderer::SetGammaExponent(float gamma)
	{
		m_Singleton->m_GammaExponent = glm::max(0.001f, gamma);
	}

	void Renderer::SetExposure(float exposure)
	{
		m_Singleton->m_HDRExposure = exposure;
	}

	void Renderer::EnableBloom(bool enable)
	{
		m_Singleton->m_bBloom = enable;
	}

	void Renderer::SetBrightThreshold(float threshold)
	{
		m_Singleton->m_BloomThreshold = glm::max(0.f, threshold);
	}

	void Renderer::StartFrame(float time)
	{
		if (m_Camera)
		{
			// Update data relating to the camera
			m_CameraUniform.UpdateBuffer(*m_Camera);
		}

		// Update data relating to the application
		m_AppUniform.UpdateBuffer(time, glm::ivec2(m_Width, m_Height));

		// Keep viewport to size of frame buffers
		glViewport(0, 0, m_Width, m_Height);
	}

	void Renderer::StartGeometryPass()
	{
		m_GBuffer.Bind();

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

		// By default, no stencil testing
		// Can still be used if desired
		glDisable(GL_STENCIL_TEST);

		// Reset stencil operations for potential stencil testing
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

		// No blending allowed during geometry pass
		glDisable(GL_BLEND);

		// Disable culling if wireframe is active
		// so all geometry is visible from all sides
		if (m_bRenderWireframe)
		{
			glDisable(GL_CULL_FACE);
		}
		else
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}
		
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glPolygonMode(GL_FRONT_AND_BACK, m_bRenderWireframe ? GL_LINE : GL_FILL);
	}

	void Renderer::StartLightingPass()
	{
		m_GBuffer.Unbind();

		// We need to depth values for lighting calculations
		FrameBuffer::Blit(m_GBuffer, m_LBuffer, eBufferBit::Depth);

		m_LBuffer.Bind();

		if (m_bRenderWireframe)
		{
			// Clear screen to white, so 'everything' is lit
			glClearColor(1.f, 1.f, 1.f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT);

			// No lighting during wireframe, so 
			// all lines can be visibly seen
			return;
		}

		// Clear screen to black, so areas that do not
		// recieve light default to being unlit
		glClearColor(0.2f, 0.2f, 0.2f, 1.f);	// 0.2f to act as temporary ambient lighting
		glClear(GL_COLOR_BUFFER_BIT);

		// We need to perform depth testing but
		// dont need to write to the depth buffer
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);

		// Prebind textures used by lighting
		m_GBuffer.BindTarget(m_PosIdx, m_PosIdx);
		m_GBuffer.BindTarget(m_NorIdx, m_NorIdx);
		m_GBuffer.BindTarget(m_AlbIdx, m_AlbIdx);

		m_PNTShader.Bind();
		
		// Enable stencil testing for stencil pass
		glEnable(GL_STENCIL_TEST);

		// Enable additive blending for lighting pass
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glBindVertexArray(m_sphereVAO);
		for (int32 i = 0; i < m_LightUniform.GetPntCount(); ++i)
		{
			// Stencil pass
			{
				glEnable(GL_DEPTH_TEST);

				glDisable(GL_CULL_FACE);

				// Clear stencil buffer for every test
				glClear(GL_STENCIL_BUFFER_BIT);

				// We always want stencil test to succeed
				// Only the depth test matters
				glStencilFunc(GL_ALWAYS, 0, 0x00);

				// Back face renders will increment stencil buffer
				glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);

				// Front face renders will decrement stencil buffer
				glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

				m_SShader.Bind();
				m_SShader.SetUniformValue("index", i);
				glDrawElements(GL_TRIANGLE_STRIP, m_sphereIndices, GL_UNSIGNED_INT, 0);
			}

			// Lighting pass
			{
				glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

				glDisable(GL_DEPTH_TEST);

				// No longer need both faces
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT); 

				m_PNTShader.Bind();
				m_PNTShader.SetUniformValue("index", i);
				glDrawElements(GL_TRIANGLE_STRIP, m_sphereIndices, GL_UNSIGNED_INT, 0);
			}
		}

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);

		// Re-enable back face culling
		glCullFace(GL_BACK);

		m_DIRShader.Bind();

		glBindVertexArray(m_VAO);
		for (int32 i = 0; i < m_LightUniform.GetDirCount(); ++i)
		{
			m_DIRShader.SetUniformValue("index", i);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
		}
		glBindVertexArray(0);

		m_DIRShader.Unbind();

		// No longer need to blend
		glDisable(GL_BLEND);
	}

	void Renderer::StartPostProcessPass()
	{
		m_LBuffer.Unbind();

		// Get all of the stencil information that could
		// have been performed during the geometry pass
		//FrameBuffer::Blit(m_GBuffer, m_LBuffer, eBufferBit::Stencil);

		m_PBuffer.Bind();

		// (post buffer only has color buffers right now)
		// (might need a stencil buffer to blit stencil from geoemtry) 
		glClear(GL_COLOR_BUFFER_BIT);

		// By default, post processing needs
		// depth or stencil testing
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);

		// Always fill the quad
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		m_IPShader.Bind();	
		m_GBuffer.BindTarget(m_AlbIdx, 0);
		m_LBuffer.BindTarget(m_ColIdx, 1);

		// Update initial post pass variables
		m_IPShader.SetUniformValue("bBloom", m_bRenderWireframe ? false : m_bBloom);
		m_IPShader.SetUniformValue("brightnessThreshold", m_BloomThreshold);

		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
		glBindVertexArray(0);

		if (m_bBloom && !m_bRenderWireframe)
		{
			// Apply bloom will set the texture
			// texture to use for final pass
			ApplyBloom();
		}
		else
		{
			// Bind the brightness value which
			// should now be all black
			m_PBuffer.BindTarget(m_BrightIdx, 1);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_FPShader.Bind();
		m_PBuffer.BindTarget(m_NoPostIdx, 0);

		// Update final post pass variables
		m_FPShader.SetUniformValue("bGammaCorrect", m_bGammaCorrect);
		m_FPShader.SetUniformValue("gamma", m_GammaExponent);
		m_FPShader.SetUniformValue("exposure", m_HDRExposure);

		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
		glBindVertexArray(0);

		m_FPShader.Unbind();
	}

	void Renderer::EndFrame()
	{
		
	}

	bool Renderer::Initialize(int32 width, int32 height)
	{
		assert(width >= 0 && height >= 0);

		float vertices[16] =
		{
			-1.f, -1.f,		0.f, 0.f,
			1.f, -1.f,		1.f, 0.f,
			1.f, 1.f,		1.f, 1.f,
			-1.f, 1.f,		0.f, 1.f
		};
		byte indices[6] =
		{
			0, 1, 2,
			0, 2, 3
		};

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(byte) * 6, indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		GenerateSphere();

		m_PosIdx = m_GBuffer.AttachTexture2D(eTargetFormat::RGB16F);		// TODO: replace with depth values
		m_NorIdx = m_GBuffer.AttachTexture2D(eTargetFormat::RGB16F);		// World Normals
		m_AlbIdx = m_GBuffer.AttachTexture2D(eTargetFormat::RGBA);			// Albedo + specular
		m_GBuffer.AttachRenderBuffer(eTargetFormat::Depth24Stencil8);		// Required depth + custom stencil

		assert(m_GBuffer.Create());

		m_ColIdx = m_LBuffer.AttachTexture2D(eTargetFormat::RGB16F);		// HDR lighting
		m_LBuffer.AttachRenderBuffer(eTargetFormat::Depth24Stencil8);		// Required depth and stencil

		assert(m_LBuffer.Create());

		m_NoPostIdx = m_PBuffer.AttachTexture2D(eTargetFormat::RGB16F);		// AlbedoSpec + HDR lighting
		m_BrightIdx = m_PBuffer.AttachTexture2D(eTargetFormat::RGB16F);		// NoPost above brightness threshold
		// TODO: add stencil render buffer
			
		assert(m_PBuffer.Create());

		m_PingBuffer.AttachTexture2D(eTargetFormat::RGB16F, eFilterMode::Linear);	
		m_PongBuffer.AttachTexture2D(eTargetFormat::RGB16F, eFilterMode::Linear);

		assert(m_PingBuffer.Create() && m_PongBuffer.Create());

		m_SShader.Load("Resources/Shaders/PntLightStencilPass.vert", "Resources/Shaders/PntLightStencilPass.frag");

		m_DIRShader.Load("Resources/Shaders/DirLight.vert", "Resources/Shaders/DirLight.frag");
		m_PNTShader.Load("Resources/Shaders/PntLight.vert", "Resources/Shaders/PntLight.frag");

		m_DIRShader.Bind();
		m_DIRShader.SetUniformValue("target.gPosition", m_PosIdx);
		m_DIRShader.SetUniformValue("target.gNormal", m_NorIdx);
		m_DIRShader.SetUniformValue("target.gAlbedoSpec", m_AlbIdx);

		m_PNTShader.Bind();
		m_PNTShader.SetUniformValue("target.gPosition", m_PosIdx);
		m_PNTShader.SetUniformValue("target.gNormal", m_NorIdx);
		m_PNTShader.SetUniformValue("target.gAlbedoSpec", m_AlbIdx);
	
		m_IPShader.Load("Resources/Shaders/PostProcess/InitPost.vert", "Resources/Shaders/PostProcess/InitPost.frag");
		m_FPShader.Load("Resources/Shaders/PostProcess.vert", "Resources/Shaders/PostProcess.frag");

		m_BloomShader.Load("Resources/Shaders/PostProcess/Bloom.vert", "Resources/Shaders/PostProcess/Bloom.frag");

		m_IPShader.Bind();
		m_IPShader.SetUniformValue("target.gAlbedoSpec", 0);
		m_IPShader.SetUniformValue("target.lColor", 1);	

		m_FPShader.Bind();
		m_FPShader.SetUniformValue("target.pNoPost", 0);
		m_FPShader.SetUniformValue("target.pBloom", 1);
		m_FPShader.Unbind();

		return true;
	}

	bool Renderer::Cleanup()
	{
		m_GBuffer.Destroy();
		m_LBuffer.Destroy();
		m_PBuffer.Destroy();
		m_PingBuffer.Destroy();
		m_PongBuffer.Destroy();

		glDeleteVertexArrays(1, &m_VAO);
		glDeleteVertexArrays(1, &m_sphereVAO);

		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_IBO);
		glDeleteBuffers(1, &m_sphereVBO);
		glDeleteBuffers(1, &m_sphereIBO);

		return true;
	}

	void Renderer::Create(int32 width, int32 height)
	{
		if (!m_Singleton)
		{
			m_Singleton = new Renderer(width, height);
			m_Singleton->Initialize(width, height);
		}
	}

	void Renderer::Destroy()
	{
		if (m_Singleton)
		{
			m_Singleton->Cleanup();
			delete m_Singleton;
		}
	}

	void Renderer::ApplyBloom()
	{
		bool hor = true;
		int32 amount = 10;

		m_BloomShader.Bind();
		m_BloomShader.SetUniformValue("scene", 0);

		// Start with our initial brightness
		m_PBuffer.BindTarget(m_BrightIdx, 0);

		glBindVertexArray(m_VAO);
		for (int32 i = 0; i < amount; ++i)
		{
			if (hor)
				m_PingBuffer.Bind();
			else
				m_PongBuffer.Bind();

			m_BloomShader.SetUniformValue("horizontal", hor);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

			if (hor)
				m_PingBuffer.BindTarget(0);
			else
				m_PongBuffer.BindTarget(0);

			hor = !hor;
		}
		glBindVertexArray(0);

		if (hor)
			m_PongBuffer.BindTarget(0, 1);
		else
			m_PingBuffer.BindTarget(0, 1);
	}

	void Renderer::GenerateSphere()
	{
		glGenVertexArrays(1, &m_sphereVAO);

		glGenBuffers(1, &m_sphereVBO);
		glGenBuffers(1, &m_sphereIBO);

		std::vector<glm::vec3> positions;
		std::vector<uint32> indices;

		const uint32 X_SEGMENTS = 16;
		const uint32 Y_SEGMENTS = 16;
		for (uint32 y = 0; y <= Y_SEGMENTS; ++y)
		{
			for (uint32 x = 0; x <= X_SEGMENTS; ++x)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = glm::cos(xSegment * 2.0f * glm::pi<float>()) * glm::sin(ySegment * glm::pi<float>());
				float yPos = glm::cos(ySegment * glm::pi<float>());
				float zPos = glm::sin(xSegment * 2.0f * glm::pi<float>()) * glm::sin(ySegment * glm::pi<float>());

				positions.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (int32 y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (int32 x = 0; x <= X_SEGMENTS; ++x)
				{				
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back(y       * (X_SEGMENTS + 1) + x); 
				}
			}
			else
			{
				for (int32 x = X_SEGMENTS; x >= 0; --x)
				{									
					indices.push_back(y       * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
		
		m_sphereIndices = indices.size();

		glBindVertexArray(m_sphereVAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_sphereVBO);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_sphereIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}