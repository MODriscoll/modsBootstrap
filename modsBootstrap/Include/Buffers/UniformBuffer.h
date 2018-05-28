#pragma once

#include "Buffer.h"

namespace mods
{
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