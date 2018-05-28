#include "Buffers\UniformBuffer.h"

#include "IncludeGLFW.h"

#include <cassert>
#include <iostream>

namespace mods
{
	UniformBuffer::UniformBuffer(uint32 size, int32 binding, void* data)
		: m_Handle(0)
		, m_Size(size)
		, m_Binding(binding)
	{
		Create(size, data);
	}

	UniformBuffer::UniformBuffer(UniformBuffer&& rhs)
		: m_Handle(rhs.m_Handle)
		, m_Size(rhs.m_Size)
		, m_Binding(rhs.m_Binding)
	{
		rhs.m_Handle = 0;
		rhs.m_Size = 0;
		rhs.m_Binding = 0;
	}

	UniformBuffer::~UniformBuffer()
	{
		Destroy();
	}

	UniformBuffer& UniformBuffer::operator=(UniformBuffer&& rhs)
	{
		// Destroy potential existing buffer
		Destroy();

		m_Handle = rhs.m_Handle;
		m_Size = rhs.m_Size;
		m_Binding = rhs.m_Binding;

		rhs.m_Handle = 0;
		rhs.m_Size = 0;
		rhs.m_Binding = 0;

		return *this;
	}

	void UniformBuffer::Fill(uint32 start, uint32 amount, void* data) const
	{
		assert(IsValid());
		assert(start >= 0 && start < m_Size);
		
		/*if ((start + amount) > m_Size)
		{
			#if _DEBUG
			std::cout << "Warning: Not enough room to fill uniform buffer with data\n"
				"Start=" << start << ", End=" << (start + amount) << ", Size=" << m_Size << std::endl;
			#endif
			amount = m_Size - start;
		}*/

		glBindBuffer(GL_UNIFORM_BUFFER, m_Handle);
		glBufferSubData(GL_UNIFORM_BUFFER, start, amount, data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void UniformBuffer::Create(uint32 size, void* data)
	{
		// 4 bytes is size of smallest value
		assert(size >= 4);

		glGenBuffers(1, &m_Handle);
		glBindBuffer(GL_UNIFORM_BUFFER, m_Handle);
		
		glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void UniformBuffer::Destroy()
	{
		if (IsValid())
			glDeleteBuffers(1, &m_Handle);
	}
}