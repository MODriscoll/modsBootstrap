#include "Buffers\CameraUniformBuffer.h"

#include "Camera\Camera.h"

namespace mods
{
	// Camera Uniform		binding = 0
	// mat4 projection		(64 bytes)	(offset=0)
	// mat4 view			(64 bytes)	(offset=64)
	// vec3 position		(16 bytes)	(offset=128)
	// vec3 heading			(16 bytes)	(offset=144)

	// final size			160 bytes

	struct CameraBuffer
	{
	public:

		CameraBuffer(const Camera& camera)
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

			float Data[40];
		};
	};

	CameraUniforms::CameraUniforms()
		: UniformBuffer(160, 0)
	{

	}

	void CameraUniforms::UpdateBuffer(const Camera& camera) const
	{
		// Convert to aligned data, so we can pass with only one call
		CameraBuffer buffer(camera);
		Fill(0, 160, buffer.Data);
	}
}