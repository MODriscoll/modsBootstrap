#include "Buffers\CameraUniformBuffer.h"

#include "Camera\Camera.h"

namespace mods
{
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
		: UniformBuffer(sizeof(CameraBuffer), 0)
	{

	}

	void CameraUniforms::UpdateBuffer(const Camera& camera) const
	{
		// Convert to aligned data, so we can pass with only one call
		CameraBuffer buffer(camera);
		Fill(0, sizeof(CameraBuffer), buffer.Data);
	}
}