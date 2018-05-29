#include "Application\Renderer.h"

#include "IncludeGLFW.h"
#include "Rendering\Lighting\DirectionalLight.h"
#include "Rendering\Lighting\SpotLight.h"

#include <glm\gtc\type_ptr.hpp>

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

	Renderer::Renderer(int32 width, int32 height)
		: m_FBO(0)
		, m_RBO(0)
		, m_DirCount(0)
		, m_PntCount(0)
		, m_SptCount(0)
	{
		
	}

	Renderer::Renderer(Renderer&& rhs)
		: m_FBO(rhs.m_FBO)
		, m_RBO(rhs.m_RBO)
		, m_PosBuffer(std::move(rhs.m_PosBuffer))
		, m_NorBuffer(std::move(rhs.m_NorBuffer))
		, m_AlbBuffer(std::move(rhs.m_AlbBuffer))
	{
		rhs.m_FBO = 0;
		rhs.m_RBO = 0;
	}

	Renderer::~Renderer()
	{
		Cleanup();
	}

	Renderer& Renderer::operator=(Renderer&& rhs)
	{
		// Clear any existing buffer
		Cleanup();

		m_FBO = rhs.m_FBO;
		m_RBO = rhs.m_RBO;
		m_PosBuffer = std::move(rhs.m_PosBuffer);
		m_NorBuffer = std::move(rhs.m_NorBuffer);
		m_AlbBuffer = std::move(rhs.m_AlbBuffer);

		rhs.m_FBO = 0;
		rhs.m_RBO = 0;

		return *this;
	}

	void Renderer::DrawMesh(const Mesh& mesh, const glm::mat4x4& transform)
	{
		
	}

	void Renderer::DrawModel(const Model& model, const glm::mat4x4& transform)
	{
		
	}

	int32 Renderer::AddLight(const Light& light)
	{
		switch (light.GetLightType())
		{
			case eLightType::Directional:
			{

			}

			case eLightType::Point:
			{

			}

			case eLightType::Spot:
			{

			}

			default:
			{
				assert(false);
			}
		}

		return -1;
	}

	bool Renderer::UpdateLight(const Light& light, int32 index)
	{
		return true;
	}

	void Renderer::StartFrame()
	{
	}

	void Renderer::StartGeometryPass()
	{
	}

	void Renderer::StartLightingPass()
	{
	}

	void Renderer::StartPostProcessPass()
	{
	}

	void Renderer::EndFrame()
	{
	}

	bool Renderer::Initialize(int32 width, int32 height)
	{
		assert(width >= 0 && height >= 0);

		// Generate geometry buffer
		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		// Generate textures to write geometry data to
		// TODO: make it so the internal format size (float16, float32 can be set for textures)
		m_PosBuffer.Create(width, height, eTextureChannels::RGB);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PosBuffer.GetHandle(), 0);

		m_NorBuffer.Create(width, height, eTextureChannels::RGB);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_NorBuffer.GetHandle(), 0);

		// Diffuse stored in RGB, specular in alpha
		m_AlbBuffer.Create(width, height, eTextureChannels::RGBA);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_AlbBuffer.GetHandle(), 0);

		uint32 buffers[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

		// Set the textures as the draw buffers
		glDrawBuffers(3, buffers);

		// Generate the depth and stencil buffer
		glGenRenderbuffers(1, &m_RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);

		// Generate buffer to store both depth and stencil values
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Error: Failed to generate frame buffer" << std::endl;

			Cleanup();

			return false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return true;
	}

	bool Renderer::Cleanup()
	{
		if (m_FBO != 0)
		{
			glDeleteFramebuffers(1, &m_FBO);
			glDeleteRenderbuffers(1, &m_RBO);

			m_PosBuffer.Destroy();
			m_NorBuffer.Destroy();
			m_AlbBuffer.Destroy();
		}

		return true;
	}

	int32 Renderer::AddDirectionalLight(const DirectionalLight& light)
	{
		if (m_DirCount >= m_DirLights.size())
			return -1;

		m_DirLights[m_DirCount] = DirectionalLightData(light);
		return m_DirCount++;
	}

	int32 Renderer::AddPointLight(const PointLight& light)
	{
		//if (m_PntCount >= m_PntLights.size())
		//	return -1;

		//m_PntLights[m_PntCount] = PointLightData(light);
		return m_PntCount++;
	}

	int32 Renderer::AddSpotLight(const SpotLight& light)
	{
		//if (m_SptCount >= m_SptLights.size())
		//	return -1;

		//m_SptLights[m_SptCount] = SpotLightData(light);
		return m_SptCount++;
	}
}