#include "Buffers\AppUniformBuffer.h"

namespace mods
{
	struct AppBuffer
	{
	public:

		AppBuffer(float time, const glm::ivec2& viewportsize)
			: Time(time)
			, ViewportSize(viewportsize)
		{

		}

	public:

		union 
		{
			struct
			{
				glm::vec2 ViewportSize;
				float Time;
			};

			float Data[3];
		};
	};

	AppUniforms::AppUniforms()
		: UniformBuffer(sizeof(AppBuffer), 2)
	{

	}

	// TODO: viewport size does not need to be constantly updated
	void AppUniforms::UpdateBuffer(float time, const glm::ivec2& viewportsize)
	{
		// Convert to aligned data, so we can pass with only one call
		AppBuffer buffer(time, viewportsize);
		Fill(0, sizeof(AppBuffer), buffer.Data);
	}
}
