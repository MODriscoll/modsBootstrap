#include "Rendering\Particles\ParticleEmitter.h"

#include "IncludeGLFW.h"

#include <glm\geometric.hpp>

namespace mods
{
	ParticleEmitter::ParticleEmitter()
		: m_FirstInactive(0)
		, m_VAO(0)
		, m_VBO(0)
		, m_IBO(0)
		, m_Position(0.f)
		, m_EmitTimer(0.f)
		, m_EmitRate(1.f)
		, m_LifespanMin(1.f)
		, m_LifespanMax(2.f)
		, m_VelocityMin(1.f)
		, m_VelocityMax(2.f)
		, m_StartSize(1.f)
		, m_EndSize(0.f)
		, m_StartColor(0.f)
		, m_EndColor(0.f)
	{

	}

	ParticleEmitter::~ParticleEmitter()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_IBO);
	}

	void ParticleEmitter::Initialise(
		uint32 maxparticles, uint32 emitrate, float lifetimemin, float lifetimemax, 
		float velocitymin, float velocitymax, float startsize, float endsize, 
		const glm::vec3& startcolor, const glm::vec3& endcolor)
	{
		m_EmitTimer = 0.f;
		m_EmitRate = 1.f / (float)emitrate;

		m_StartColor = startcolor;
		m_EndColor = endcolor;
		m_StartSize = startsize;
		m_EndSize = endsize;
		m_VelocityMin = velocitymin;
		m_VelocityMax = velocitymax;
		m_LifespanMin = lifetimemin;
		m_LifespanMax = lifetimemax;

		m_Particles.resize(maxparticles);
		m_FirstInactive = 0;

		// Quad has four vertices
		m_VertexData.resize(maxparticles * 4);

		// Indices for quad
		uint32* indexData = new uint32[maxparticles * 6];
		for (uint32 i = 0; i < maxparticles; ++i)
		{
			indexData[i * 6 + 0] = i * 4 + 0;
			indexData[i * 6 + 1] = i * 4 + 1;
			indexData[i * 6 + 2] = i * 4 + 2;

			indexData[i * 6 + 3] = i * 4 + 0;
			indexData[i * 6 + 4] = i * 4 + 2;
			indexData[i * 6 + 5] = i * 4 + 3;
		}

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, maxparticles * 4 * sizeof(ParticleVertex), m_VertexData.data(), GL_DYNAMIC_DRAW);

		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxparticles * 6 * sizeof(uint32), indexData, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (void*)offsetof(ParticleVertex, Position));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (void*)offsetof(ParticleVertex, Color));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (void*)offsetof(ParticleVertex, Normal));
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (void*)offsetof(ParticleVertex, TexCoord));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		delete[] indexData;
	}

	void ParticleEmitter::Update(float deltaTime, const glm::mat4& camtransform)
	{
		m_EmitTimer += deltaTime;
		while (m_EmitTimer >= m_EmitRate)
		{
			Emit();
			m_EmitTimer -= m_EmitRate;
		}

		uint32 quad = 0;

		for (uint32 i = 0; i < m_FirstInactive; ++i)
		{
			Particle* particle = &m_Particles[i];

			particle->Lifetime += deltaTime;
			if (particle->Lifetime >= particle->Lifespan)
			{
				*particle = m_Particles[m_FirstInactive - 1];
				m_FirstInactive--;
			}
			else
			{
				particle->Position += particle->Velocity * deltaTime;

				particle->Size = glm::mix(m_StartSize, m_EndSize, particle->Lifetime / particle->Lifespan);
				particle->Color = glm::mix(m_StartColor, m_EndColor, particle->Lifetime / particle->Lifespan);

				// make a quad the correct size and color
				float halfsize = particle->Size * 0.5f;

				m_VertexData[quad * 4 + 0].Position = glm::vec3(halfsize, halfsize, 0.f);
				m_VertexData[quad * 4 + 0].Color = particle->Color;
				m_VertexData[quad * 4 + 0].TexCoord = glm::vec2(1.f, 1.f);

				m_VertexData[quad * 4 + 1].Position = glm::vec3(-halfsize, halfsize, 0.f);
				m_VertexData[quad * 4 + 1].Color = particle->Color;
				m_VertexData[quad * 4 + 1].TexCoord = glm::vec2(0.f, 1.f);

				m_VertexData[quad * 4 + 2].Position = glm::vec3(-halfsize, -halfsize, 0.f);
				m_VertexData[quad * 4 + 2].Color = particle->Color;
				m_VertexData[quad * 4 + 2].TexCoord = glm::vec2(0.f, 0.f);

				m_VertexData[quad * 4 + 3].Position = glm::vec3(halfsize, -halfsize, 0.f);
				m_VertexData[quad * 4 + 3].Color = particle->Color;
				m_VertexData[quad * 4 + 3].TexCoord = glm::vec2(1.f, 0.f);

				// Create billboard transform
				glm::vec3 zAxis = glm::normalize(glm::vec3(camtransform[3]) - particle->Position);
				glm::vec3 xAxis = glm::cross(glm::vec3(camtransform[1]), zAxis);
				glm::vec3 yAxis = glm::cross(zAxis, xAxis);

				glm::mat4 billboard(
					glm::vec4(xAxis, 0.f),
					glm::vec4(yAxis, 0.f),
					glm::vec4(zAxis, 0.f),
					glm::vec4(0.f, 0.f, 0.f, 1.f));

				m_VertexData[quad * 4 + 0].Position = glm::vec3(billboard * glm::vec4(m_VertexData[quad * 4 + 0].Position, 1.f)) + particle->Position;
				m_VertexData[quad * 4 + 1].Position = glm::vec3(billboard * glm::vec4(m_VertexData[quad * 4 + 1].Position, 1.f)) + particle->Position;
				m_VertexData[quad * 4 + 2].Position = glm::vec3(billboard * glm::vec4(m_VertexData[quad * 4 + 2].Position, 1.f)) + particle->Position;
				m_VertexData[quad * 4 + 3].Position = glm::vec3(billboard * glm::vec4(m_VertexData[quad * 4 + 3].Position, 1.f)) + particle->Position;

				m_VertexData[quad * 4 + 0].Normal = zAxis;
				m_VertexData[quad * 4 + 1].Normal = zAxis;
				m_VertexData[quad * 4 + 2].Normal = zAxis;
				m_VertexData[quad * 4 + 3].Normal = zAxis;

				++quad;
			}
		}
	}

	void ParticleEmitter::Draw()
	{
		// sync the particle vertex buffer
		// based on how many alive particles there are
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_FirstInactive * 4 * sizeof(ParticleVertex), m_VertexData.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// draw particles
		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, m_FirstInactive * 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void ParticleEmitter::Emit()
	{
		// Can only emit dead particles
		if (m_FirstInactive >= Count())
			return;

		// resurrect dead particle
		Particle& particle = m_Particles[m_FirstInactive++];

		particle.Position = m_Position;

		// Random lifespan
		particle.Lifetime = 0.f;
		particle.Lifespan = (rand() / (float)RAND_MAX) * (m_LifespanMax - m_LifespanMin) + m_LifespanMin;

		particle.Color = m_StartColor;
		particle.Size = m_StartSize;

		float velocity = (rand() / (float)RAND_MAX) * (m_VelocityMax - m_VelocityMin) + m_VelocityMin;

		particle.Velocity.x = (rand() / (float)RAND_MAX) * 2 - 1;
		particle.Velocity.y = (rand() / (float)RAND_MAX) * 2 - 1;
		particle.Velocity.z = (rand() / (float)RAND_MAX) * 2 - 1;

		particle.Velocity = glm::normalize(particle.Velocity) * velocity;
	}
}