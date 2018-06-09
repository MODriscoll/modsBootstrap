#pragma once

#include "Buffer.h"

namespace mods
{
	// Types work by alignment, meaning they are placed
	// at certain strides instead of one after the other

	// Scalars (bool, int, float) are every 4 bytes
	// Vec2 (all types) are every 8 bytes
	// Vec3 and Vec4 (all types) are every 16 bytes
	// Matrices are treated as arrays of Vec4s, meaing
	// a mat4 = vec4[4], so 64 bytes in total

	// Custom structs follow the same rules as above,
	// but are padded to a multiple of a vec3/vec4

	// Examples
	// float myfirstfloat;		// 0 offset
	// float mysecond float		// 4 offset

	// Vec3s are aligned at every 16 bytes,
	// so we need to pad an additional two floats
	// float pad1;				// 8 offset
	// float pad2;				// 12 offset

	// vec3 myvector;			// 16 offset

	// First iteration of uniform buffer
	struct UniformBuffer
	{
	public:

		UniformBuffer(uint32 size, int32 binding, void* data = nullptr);
		UniformBuffer(const UniformBuffer& rhs) = delete;
		UniformBuffer(UniformBuffer&& rhs);

		virtual ~UniformBuffer();

		UniformBuffer& operator = (const UniformBuffer& rhs) = delete;
		UniformBuffer& operator = (UniformBuffer&& rhs);

	public:

		// Fills the buffer with the given data
		// Start = offset from beggining
		// Amount = size of data in bytes
		void Fill(uint32 start, uint32 amount, void* data) const;

	public:

		inline bool IsValid() const { return m_Handle != 0; }

	private:

		// Creates a new buffer of given size
		void Create(uint32 size, void* data);

		// Destroys the existing buffer if any
		void Destroy();

	protected:

		// Handle to the buffer
		uint32 m_Handle;

		// The size of the buffer
		uint32 m_Size;

		// The binding of the uniform
		int32 m_Binding;
	};
}