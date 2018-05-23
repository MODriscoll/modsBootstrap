#include "Application\Renderer.h"

#include "IncludeGLFW.h"
#include "Camera\Camera.h"

#include <glm\gtc\type_ptr.hpp>

namespace mods
{
	// Camera Uniform		binding = 0
	// mat4 projection		(64 bytes)	(offset=0)
	// mat4 view			(64 bytes)	(offset=64)
	// vec3 position		(16 bytes)	(offset=128)
	// vec3 view			(16 bytes)	(offset=144)

	// final size			160 bytes

	CameraUniform::CameraUniform()
		: UniformBuffer(160, 0)
	{

	}

	void CameraUniform::SetUniforms(const Camera& camera)
	{
		struct CameraData
		{
		public:

			CameraData(const Camera& camera)
				: Projection(camera.GetProjectionMatrix())
				, View(camera.GetViewMatrix())
				, Position(camera.Position, 1.f)
				, Heading(camera.GetHeading(), 0.f)
			{

			}

			
		public:

			union
			{
				struct
				{
					glm::mat4 Projection;
					glm::mat4 View;
					glm::vec4 Position;
					glm::vec4 Heading;
				};

				// Pointer to raw values
				float Raw[40];
			};
		};

		CameraData data(camera);
		Fill(0, m_Size, data.Raw);
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