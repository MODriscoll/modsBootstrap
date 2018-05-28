#include "Application\Renderer.h"

#include "IncludeGLFW.h"
#include "Camera\Camera.h"
#include "Rendering\Meshes\Model.h"

#include <glm\gtc\type_ptr.hpp>

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

	void Renderer::DrawMesh(const Mesh& mesh, const glm::mat4x4& transform)
	{
		m_GShader.Bind();
		m_GShader.SetUniformValue("model", transform);
		mesh.Draw(m_GShader);
		m_GShader.Unbind();
	}

	void Renderer::DrawModel(const Model& model, const glm::mat4x4& transform)
	{
		m_GShader.Bind();
		m_GShader.SetUniformValue("model", transform);
		model.Draw(m_GShader);
		m_GShader.Unbind();
	}

	void Renderer::AddLight(const Light& light)
	{
	}

	Renderer::Renderer()
	{
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::Initialize(int32 width, int32 height)
	{
		std::vector<eTextureBufferTypes> types;
		types.push_back(eTextureBufferTypes::RGB); // position
		types.push_back(eTextureBufferTypes::RGB); // normals
		types.push_back(eTextureBufferTypes::RGBA); // albedo (diffuse (rgb) + specular (a))

		m_GBuffer.Create(width, height, types);

		m_GShader.Load("Resources/Shaders/vGShader.vert", "Resources/Shaders/fGShader.frag");
		m_LShader.Load("Resources/Shaders/vLShader.vert", "Resources/Shaders/fLShader.frag");
		m_PShader.Load("Resources/Shaders/vPShader.vert", "Resources/Shaders/fPShader.frag");
	}

	void Renderer::Cleanup()
	{
		
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
}