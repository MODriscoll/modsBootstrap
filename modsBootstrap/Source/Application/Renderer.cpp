#include "Application\Renderer.h"

#include "IncludeGLFW.h"
#include "Rendering\Lighting\DirectionalLight.h"
#include "Rendering\Lighting\SpotLight.h"

#include "Rendering\Meshes\Model.h"

#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>

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
		: m_GTarget(width, height)
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
				const SpotLight& sptlight = static_cast<const SpotLight&>(light);
				return m_Singleton->m_LightUniform.AddSpotLight(SpotLightData(sptlight));
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
				break;
			}

			case eLightType::Point:
			{
				break;
			}

			case eLightType::Spot:
			{
				const SpotLight& sptlight = static_cast<const SpotLight&>(light);
				m_Singleton->m_LightUniform.UpdateSpotLight(SpotLightData(sptlight), index);
				break;
			}

			default:
			{
				assert(false);
			}
		}
	}

	void Renderer::StartFrame()
	{
	}

	void Renderer::StartGeometryPass()
	{
		m_GTarget.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
	}

	void Renderer::StartLightingPass()
	{
		m_GTarget.Unbind();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);

		m_LShader.Bind();
		m_GTarget.GetTarget(m_PosIdx).Bind(m_PosIdx);
		m_GTarget.GetTarget(m_NorIdx).Bind(m_NorIdx);
		m_GTarget.GetTarget(m_AlbIdx).Bind(m_AlbIdx);

		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
		glBindVertexArray(0);
	}

	void Renderer::StartPostProcessPass()
	{
		// Make function in render target
		{
			glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GTarget.GetHandle());
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

			// TODO: need to track size of viewport
			// Copy the depth and stencil values to be potenially used for post processing
			glBlitFramebuffer(0, 0, m_GTarget.GetWidth(), m_GTarget.GetHeight(), 0, 0, m_GTarget.GetWidth(), m_GTarget.GetHeight(), GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

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

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

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

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_PosIdx = m_GTarget.AttachTarget(eTextureFormat::RGB16F);
		m_NorIdx = m_GTarget.AttachTarget(eTextureFormat::RGB16F);
		m_AlbIdx = m_GTarget.AttachTarget(eTextureFormat::RGBA);

		assert(m_GTarget.Create());

		m_LShader.Load("Resources/Shaders/GLight.vert", "Resources/Shaders/GLight.frag");

		m_LShader.Bind();
		m_LShader.SetUniformValue("target.gPosition", m_PosIdx);
		m_LShader.SetUniformValue("target.gNormal", m_NorIdx);
		m_LShader.SetUniformValue("target.gAlbedoSpec", m_AlbIdx);
		m_LShader.Unbind();

		return true;
	}

	bool Renderer::Cleanup()
	{
		return m_GTarget.Destroy();
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
}