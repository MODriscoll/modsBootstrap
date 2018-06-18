#include "Rendering\Particles\GPUParticleSystem.h"

#include "IncludeGLFW.h"
#include "Rendering\Shaders\Shader.h"
#include "Rendering\Shaders\ShaderUtility.h"

#include <glm\geometric.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>

namespace mods
{
	GPUParticleSystem::~GPUParticleSystem()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(3, m_SSBs);
		glDeleteProgram(m_Program);
	}

	void GPUParticleSystem::Init(int32 MaxParticles)
	{
		if (MaxParticles <= 0)
			return;

		m_Particles = (1024 * 1024);// MaxParticles;

		m_Positions.resize(m_Particles, glm::vec4(0.f));
		m_Velocities.resize(m_Particles, glm::vec4(0.f));
		m_Colors.resize(m_Particles, glm::vec4(1.f));

		glGenBuffers(3, m_SSBs);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBs[0]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * m_Particles, nullptr, GL_DYNAMIC_COPY);

		glm::vec4* points = (glm::vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_Particles * sizeof(glm::vec4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		for (int32 i = 0; i < m_Particles; ++i)
		{
			points[i] = m_Positions[i];
		}

		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

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

		std::string source;
		assert(detail::LoadShaderFromSource("Resources/Shaders/c.cmpt", source));

		const char* sc = source.c_str();
		uint32 shader = glCreateShader(GL_COMPUTE_SHADER);
		glShaderSource(shader, 1, &sc, nullptr);
		glCompileShader(shader);

		// Handle any potential errors
		int32 success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char log[512];
			glGetShaderInfoLog(shader, 512, nullptr, log);

			std::cout << "Error: Failed to compile shader. Log:\n" << log << std::endl;

			// Destroy failed shader
			glDeleteShader(shader);

			assert(false);
		}

		m_Program = glCreateProgram();
		glAttachShader(m_Program, shader);
		glLinkProgram(m_Program);

		// Handle potential errors
		glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
		if (!success)
		{
			char log[512];
			glGetProgramInfoLog(m_Program, 512, nullptr, log);

			std::cout << "Error: Failed to link program. Log:\n" << log << std::endl;

			// Delete failed program and shaders
			glDeleteProgram(m_Program);
			glDeleteShader(shader);

			assert(false);
		}

		glDetachShader(m_Program, shader);
		glDeleteShader(shader);

		glGenVertexArrays(1, &m_VAO);
	}

	void GPUParticleSystem::Update()
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_SSBs[0]);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, m_SSBs[1]);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, m_SSBs[2]);

		glUseProgram(m_Program);
		glDispatchCompute(m_Particles / 128, 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}

	void GPUParticleSystem::Draw(ShaderProgram& program)
	{
		program.Bind();
		program.SetUniformValue("size", 0.5f);
		program.SetUniformValue("model", glm::translate(glm::mat4(1.f), m_EmitterPosition));

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_SSBs[0]);	
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec4), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, m_SSBs[2]);
		glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(glm::vec4), (void*)0);
		glEnableVertexAttribArray(1);

		glDrawArrays(GL_POINTS, 0, m_Particles);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		program.Unbind();
	}
}