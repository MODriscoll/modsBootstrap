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

	// Camera Uniform		binding = 0
	// mat4 projection		(64 bytes)	(offset=0)
	// mat4 view			(64 bytes)	(offset=64)
	// vec3 position		(16 bytes)	(offset=128)
	// vec3 heading			(16 bytes)	(offset=144)

	// final size			160 bytes

	// STRUCTS get padded to be the multiplication of a vec4 (vec4 being 16 bytes)

	// struct directionallight
	// vec4 color				(16 bytes)	(offset=0)
	// float ambientstrength	(4 bytes)	(offset=16)
	// float diffusestrength	(4 bytes)	(offset=20)
	// vec3 direction			(16 bytes)	(offset=24)

	// final size				40 bytes
	// padded size				48 bytes

	// struct pointlight			
	// vec4 color				(16 bytes)	(offset=0)
	// float ambientstrength	(4 bytes)	(offset=16)
	// float diffusestrength	(4 bytes)	(offset=20)
	// vec3 position			(16 bytes)	(offset=24)
	// float constant			(4 bytes)	(offset=40)
	// float linear				(4 bytes)	(offset=44)
	// float quadratic			(4 bytes)	(offset=48)

	// final size				52 bytes
	// padded size				64 bytes

	// struct spotlight	
	// vec4 color				(16 bytes)	(offset=0)
	// float ambientstrength	(4 bytes)	(offset=16)
	// float diffusestrength	(4 bytes)	(offset=20)
	// vec3 position			(16 bytes)	(offset=24)
	// vec3 direction			(16 bytes)	(offset=40)
	// float innercutoff		(4 bytes)	(offset=56)
	// float outercutoff		(4 bytes)	(offset=60)
	// float constant			(4 bytes)	(offset=64)
	// float linear				(4 bytes)	(offset=68)
	// float quadratic			(4 bytes)	(offset=72)

	// final size				76 bytes
	// padded size				80 bytes

	// Light Uniform							binding = 1 (using padded size)
	// directionallight dirlights[4]			(192 bytes)	(offset=0)
	// pointlight pntlights[10]					(640 bytes) (offset=192)
	// spotlight sptlights[10]					(800 bytes) (offset=832)
	// int dircount								(4 bytes)	(offset=1632)
	// int pntcount								(4 bytes)	(offset=1636)
	// int sptcount								(4 bytes)	(offset=1640)

	// final size								1644 bytes

	// Game Uniform			binding = 2
	// float time			(4 bytes)	(offset=0)
	
	// final size			4 bytes

	Renderer::Renderer(uint32 width, uint32 height)
		: m_bGammaCorrect(false)
		, m_GammaExponent(2.2f)
		, m_GTarget(width, height)
		, m_LTarget(width, height)
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
		m_Singleton->m_CameraUniform.UpdateBuffer(camera);
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

	void Renderer::EnableGammaCorrection(bool enable)
	{
		m_Singleton->m_bGammaCorrect = enable;
	}

	void Renderer::SetGammaExponent(float gamma)
	{
		m_Singleton->m_GammaExponent = glm::max(0.001f, gamma);
	}

	void Renderer::StartFrame()
	{
	}

	void Renderer::StartGeometryPass()
	{
		m_GTarget.Bind();

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

		// By default, no stencil testing
		// Can still be used if desired
		glDisable(GL_STENCIL_TEST);

		// No blending allowed during geometry pass
		glDisable(GL_BLEND);

		// Enable back face culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void Renderer::StartLightingPass()
	{
		m_GTarget.Unbind();

		{
			glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GTarget.GetHandle());
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_LTarget.GetHandle());

			// TODO: need to track size of viewport
			// Copy the depth values from geometry pass to use for the stencil pass
			glBlitFramebuffer(0, 0, m_GTarget.GetWidth(), m_GTarget.GetHeight(), 0, 0, m_LTarget.GetWidth(), m_LTarget.GetHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		}

		m_LTarget.Bind();

		// Only clear color
		glClear(GL_COLOR_BUFFER_BIT);

		// No longer write to the depth buffer
		glDepthMask(GL_FALSE);

		m_PNTShader.Bind();
		m_GTarget.GetTarget(m_PosIdx).Bind(m_PosIdx);
		m_GTarget.GetTarget(m_NorIdx).Bind(m_NorIdx);
		m_GTarget.GetTarget(m_AlbIdx).Bind(m_AlbIdx);

		m_PNTShader.SetUniformValue("time", (float)glfwGetTime());

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
		m_GTarget.GetTarget(m_PosIdx).Bind(m_PosIdx);
		m_GTarget.GetTarget(m_NorIdx).Bind(m_NorIdx);
		m_GTarget.GetTarget(m_AlbIdx).Bind(m_AlbIdx);

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
		m_LTarget.Unbind();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_PPShader.Bind();
		m_GTarget.GetTarget(m_PosIdx).Bind(0);
		m_GTarget.GetTarget(m_NorIdx).Bind(1);
		m_GTarget.GetTarget(m_AlbIdx).Bind(2);
		m_LTarget.GetTarget(m_ColIdx).Bind(3);

		// Only enable gamma correction during last pass
		// TODO: replace with gamma correct shader, it is the
		// shader that will do the last draw to the back buffer
		// if will take in two uniforms (bGammaCorrect and gamma (default for gamma is 2.2)
		m_PPShader.SetUniformValue("bGammaCorrect", m_bGammaCorrect);
		m_PPShader.SetUniformValue("gamma", m_GammaExponent);

		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
		glBindVertexArray(0);

		m_PPShader.Unbind();

		/*m_PShader.Bind();

		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
		glBindVertexArray(0);

		m_PShader.Unbind();*/
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

		m_PosIdx = m_GTarget.AttachTarget(eTextureFormat::RGB16F);		// Position
		m_NorIdx = m_GTarget.AttachTarget(eTextureFormat::RGB16F);		// Normal
		m_AlbIdx = m_GTarget.AttachTarget(eTextureFormat::RGBA);		// Albedo + specular

		assert(m_GTarget.Create());

		m_ColIdx = m_LTarget.AttachTarget(eTextureFormat::RGB);
		
		assert(m_LTarget.Create());

		m_SShader.Load("Resources/Shaders/PntLightStencilPass.vert", "Resources/Shaders/PntLightStencilPass.frag");

		m_DIRShader.Load("Resources/Shaders/DirLight.vert", "Resources/Shaders/DirLight.frag");
		m_PNTShader.Load("Resources/Shaders/PntLight.vert", "Resources/Shaders/PntLight.frag");

		m_DIRShader.Bind();
		m_DIRShader.SetUniformValue("target.gPosition", m_PosIdx);
		m_DIRShader.SetUniformValue("target.gNormal", m_NorIdx);

		m_PNTShader.Bind();
		m_PNTShader.SetUniformValue("target.gPosition", m_PosIdx);
		m_PNTShader.SetUniformValue("target.gNormal", m_NorIdx);
	
		m_PPShader.Load("Resources/Shaders/PostProcess.vert", "Resources/Shaders/PostProcess.frag");

		m_PPShader.Bind();
		m_PPShader.SetUniformValue("target.gPosition", 0);
		m_PPShader.SetUniformValue("target.gNormal", 1);
		m_PPShader.SetUniformValue("target.gAlbedoSpec", 2);
		m_PPShader.SetUniformValue("target.lColor", 3);
		m_PPShader.Unbind();		

		return true;
	}

	bool Renderer::Cleanup()
	{
		m_GTarget.Destroy();
		m_LTarget.Destroy();

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

	void Renderer::GenerateSphere()
	{
		glGenVertexArrays(1, &m_sphereVAO);

		glGenBuffers(1, &m_sphereVBO);
		glGenBuffers(1, &m_sphereIBO);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uv;
		std::vector<glm::vec3> normals;
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
				uv.push_back(glm::vec2(xSegment, ySegment));
				normals.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (int32 y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (int32 x = 0; x <= X_SEGMENTS; ++x)
				{				
					indices.push_back(y       * (X_SEGMENTS + 1) + x); 
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int32 x = X_SEGMENTS; x >= 0; --x)
				{					
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);	
					indices.push_back(y       * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
		
		m_sphereIndices = indices.size();

		std::vector<float> data;
		for (int32 i = 0; i < (int32)positions.size(); ++i)
		{
			data.push_back(positions[i].x);
			data.push_back(positions[i].y);
			data.push_back(positions[i].z);
			if (uv.size() > 0)
			{
				data.push_back(uv[i].x);
				data.push_back(uv[i].y);
			}
		}

		glBindVertexArray(m_sphereVAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_sphereVBO);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_sphereIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32), &indices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}