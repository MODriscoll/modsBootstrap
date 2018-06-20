#include "Rendering\Particles\GPUParticleSystem.h"

#include "IncludeGLFW.h"
#include "Rendering\Shaders\Shader.h"

#include <glm\geometric.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>

namespace mods
{
	GPUParticleSystem::~GPUParticleSystem()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(5, m_SSBs);
		glDeleteBuffers(1, &m_DeadBuffer);
		glDeleteBuffers(1, &m_DeadList);
	}

	void GPUParticleSystem::Init(int32 MaxParticles)
	{
		if (MaxParticles <= 0)
			return;

		m_Particles = (501);// MaxParticles;

		std::vector<glm::vec4> defs(m_Particles, glm::vec4(0.f));
		std::vector<glm::vec4> age(m_Particles, glm::vec4(5.f));
		std::vector<glm::vec4> size(m_Particles, glm::vec4(5.f));

		glGenBuffers(5, m_SSBs);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBs[0]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * m_Particles, defs.data(), GL_DYNAMIC_COPY);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBs[1]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * m_Particles, nullptr, GL_DYNAMIC_COPY);

		glm::vec4* velocities = (glm::vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_Particles * sizeof(glm::vec4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		for (int32 i = 0; i < m_Particles; ++i)
		{
			glm::vec4& velocity = velocities[i];

			float length = (rand() / (float)RAND_MAX) * (25.f - 1.f) + 1.f;

			velocity.x = (rand() / (float)RAND_MAX) * 2 - 1;
			velocity.y = (rand() / (float)RAND_MAX) * 2 - 1;
			velocity.z = (rand() / (float)RAND_MAX) * 2 - 1;

			velocity = glm::normalize(velocity) * length;
		}

		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBs[2]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * m_Particles, nullptr, GL_DYNAMIC_COPY);

		glm::vec4* colors = (glm::vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_Particles * sizeof(glm::vec4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		for (int32 i = 0; i < m_Particles; ++i)
		{
			glm::vec4& color = colors[i];

			color.x = (float)(rand() % 255) / 255.f;
			color.y = (float)(rand() % 255) / 255.f;
			color.z = (float)(rand() % 255) / 255.f;
		}
		
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBs[3]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * m_Particles, size.data(), GL_DYNAMIC_COPY);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBs[4]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * m_Particles, age.data(), GL_DYNAMIC_COPY);

		glGenBuffers(1, &m_DeadList);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_DeadList);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::ivec4) * m_Particles, nullptr, GL_DYNAMIC_COPY);

		glm::ivec4* indices = (glm::ivec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_Particles * sizeof(glm::ivec4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		for (int32 i = 0; i < m_Particles; ++i)
		{
			indices[i].x = i;
		}

		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		glGenBuffers(1, &m_DeadBuffer);
		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, m_DeadBuffer);

		uint32 dead = 0;
		glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(uint32), &dead, GL_DYNAMIC_READ);

		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);

		assert(m_EmitShader.Load("Resources/Shaders/Particles/emit.comp"));
		assert(m_ComputeShader.Load("Resources/Shaders/Particles/c.comp"));

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_SSBs[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec4), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, m_SSBs[2]);
		glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(glm::vec4), (void*)0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, m_SSBs[3]);
		glVertexAttribPointer(2, 1, GL_FLOAT, false, sizeof(glm::vec4), (void*)0);
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		m_EmitRate = 1.f / 100.f;
	}

	void GPUParticleSystem::Update(float deltaTime)
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_SSBs[0]);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, m_SSBs[1]);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, m_SSBs[2]);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, m_SSBs[3]);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, m_SSBs[4]);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 9, m_DeadList);

		glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 3, m_DeadBuffer);

		int32 max[3];
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &max[0]); 
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &max[1]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &max[2]);

		m_EmitTime += deltaTime;
		if (m_EmitTime >= m_EmitRate)
		{
			uint32 amounttospawn = (int32)glm::floor(m_EmitTime / m_EmitRate);

			m_EmitShader.Bind();
			m_EmitShader.SetUniformValue("AmountToSpawn", amounttospawn);
			m_EmitShader.Dispatch((amounttospawn / 128) + 1, 1, 1);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT);

			m_EmitTime = std::fmod(m_EmitTime, m_EmitRate);
		}

		m_ComputeShader.Bind();
		m_ComputeShader.SetUniformValue("DT", deltaTime);
		m_ComputeShader.Dispatch((m_Particles / 128) + 1, 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT);
	}

	void GPUParticleSystem::Draw(ShaderProgram& program)
	{
		program.Bind();
		program.SetUniformValue("model", glm::translate(glm::mat4(1.f), m_EmitterPosition));

		glBindVertexArray(m_VAO);
		glDrawArrays(GL_POINTS, 0, m_Particles);
		glBindVertexArray(0);

		program.Unbind();
	}
}