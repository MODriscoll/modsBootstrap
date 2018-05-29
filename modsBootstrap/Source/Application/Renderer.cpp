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
	// Camera Uniform		binding = 0
	// mat4 projection		(64 bytes)	(offset=0)
	// mat4 view			(64 bytes)	(offset=64)
	// vec3 position		(16 bytes)	(offset=128)
	// vec3 heading			(16 bytes)	(offset=144)

	// final size			160 bytes

	// struct directionallight
	// vec4 color				(16 bytes)	(offset=0)
	// float ambientstrength	(4 bytes)	(offset=16)
	// float diffusestrength	(4 bytes)	(offset=20)
	// vec3 direction			(16 bytes)	(offset=24)
	
	// final size				40 bytes

	// struct pointlight			
	// vec4 color				(16 bytes)	(offset=0)
	// float ambientstrength	(4 bytes)	(offset=16)
	// float diffusestrength	(4 bytes)	(offset=20)
	// vec3 position			(16 bytes)	(offset=24)
	// float constant			(4 bytes)	(offset=40)
	// float linear				(4 bytes)	(offset=44)
	// float quadratic			(4 bytes)	(offset=48)

	// final size				52 bytes

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

	// Light Uniform							binding = 1
	// directionallight dirlights[4]			(160 bytes)	(offset=0)
	// pointlight pntlights[10]					(520 bytes) (offset=160)
	// spotlight sptlights[10]					(760 bytes) (offset=680)
	// int dircount								(4 bytes)	(offset=1440)
	// int pntcount								(4 bytes)	(offset=1444)
	// int sptcount								(4 bytes)	(offset=1448)

	// final size								1452 bytes

	// Game Uniform			binding = 2
	// float time			(4 bytes)	(offset=0)
	
	// final size			4 bytes

	Renderer::Renderer(uint32 width, uint32 height)
		: m_GTarget(width, height)
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

		glm::mat3x3 normal = glm::inverseTranspose(transform);
		program.SetUniformValue("model", transform);
		program.SetUniformValue("normal", normal);

		mesh.Draw(program);
	}

	void Renderer::DrawModel(const Model& model, ShaderProgram& program, const glm::mat4x4& transform)
	{
		program.Bind();

		glm::mat3x3 normal = glm::inverseTranspose(transform);
		program.SetUniformValue("model", transform);
		program.SetUniformValue("normal", normal);

		model.Draw(program);
	}

	int32 Renderer::AddLight(const Light& light)
	{
		switch (light.GetLightType())
		{
			case eLightType::Directional:
			{
				const DirectionalLight& dirlight = static_cast<const DirectionalLight&>(light);
				m_Singleton->m_LightUniform.AddDirectionalLight(DirectionalLightData(dirlight));
			}

			case eLightType::Point:
			{
				const PointLight& pntlight = static_cast<const PointLight&>(light);
				m_Singleton->m_LightUniform.AddPointLight(PointLightData(pntlight));
			}

			case eLightType::Spot:
			{
				const SpotLight& sptlight = static_cast<const SpotLight&>(light);
				m_Singleton->m_LightUniform.AddSpotLight(SpotLightData(sptlight));
			}

			default:
			{
				assert(false);
			}
		}

		return -1;
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

		m_PShader.Bind();

		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
		glBindVertexArray(0);

		m_PShader.Unbind();
	}

	void Renderer::EndFrame()
	{
		
	}

	bool Renderer::Initialize(int32 width, int32 height)
	{
		assert(width >= 0 && height >= 0);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		float vertices[16] =
		{
			-1.f, -1.f,		0.f, 0.f,
			1.f, -1.f,		1.f, 0.f,
			1.f, 1.f,		1.f, 1.f,
			-1.f, 1.f,		0.f, 1.f
		};
		byte indices[6] =
		{
			0, 2, 1,
			0, 3, 2
		};

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(byte) * 6, indices, GL_STATIC_DRAW);
		

		m_PosIdx = m_GTarget.AttachTarget(eTextureFormat::RGB16F);
		m_NorIdx = m_GTarget.AttachTarget(eTextureFormat::RGB16F);
		m_AlbIdx = m_GTarget.AttachTarget(eTextureFormat::RGBA);

		return m_GTarget.Create();
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
			m_Singleton->Destroy();
			delete m_Singleton;
		}
	}
}