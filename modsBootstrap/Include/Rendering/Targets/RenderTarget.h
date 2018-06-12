#pragma once

#include "Types.h"

#include "IncludeGLFW.h"

namespace mods
{
	enum class eTargetType : byte
	{
		Texture2D,
		RenderBuffer
	};

	enum class eTargetFormat : int32
	{
		R					= 0x1903,
		RG					= 0x8227,
		RGB					= 0x1907,
		RGBA				= 0x1908,

		R16F				= 0x822D,
		RG16F				= 0x822F,
		RGB16F				= 0x881B,
		RGBA16F				= 0x881A,

		R32F				= 0x822E,
		RG32F				= 0x8230,
		RGB32F				= 0x8815,
		RGBA32F				= 0x8814,

		Depth				= 0x1902,
		Depth16				= 0x81A5,
		Depth24				= 0x81A6,
		Depth32				= 0x81A7,

		Depth24Stencil8		= 0x88F0
	};

	class RenderTarget
	{
	public:

		RenderTarget();
		RenderTarget(const RenderTarget& rhs) = delete;
		RenderTarget(RenderTarget&& rhs);

		virtual ~RenderTarget();

		RenderTarget& operator = (const RenderTarget& rhs) = delete;
		RenderTarget& operator = (RenderTarget&& rhs);

	public:

		// Creates new target with given type and format
		virtual bool Create(int32 width, int32 height, eTargetFormat format) = 0;

		// Destroys existing target if any
		virtual bool Destroy() = 0;

		// Attaches this target to the bound frame buffer is valid
		// Requires index for color attachments (auto incremented if color)
		// Returns if index was incremented
		virtual bool BoundToFrameBuffer(int32 index = 0) const = 0;

	public:

		// Get if this target is valid
		inline bool IsValid() const { return m_Handle != 0; }

		// Direct access to handle
		inline uint32 GetHandle() const { return m_Handle; }

		// Get the internal format of this target
		inline eTargetFormat GetFormat() const { return m_Format; }

		// Get this targets type
		virtual eTargetType GetTargetType() const = 0;

	protected:

		// Get the attachment type of this target
		uint32 GetAttachmentType(int32 index, bool& iscolor) const;

	protected:

		// Handle to the target
		uint32 m_Handle;

		// Format of the target
		eTargetFormat m_Format;
	};
}