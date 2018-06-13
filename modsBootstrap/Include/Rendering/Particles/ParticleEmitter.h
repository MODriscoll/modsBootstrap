#pragma once

#include "Types.h"

#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>

#include <vector>

namespace mods
{
	struct Particle
	{
		Particle()
			: Position(0.f)
			, Velocity(0.f)
			, Color(1.f)
			, Size(1.f)
			, Lifetime(0.f)
			, Lifespan(5.f)
		{

		}

		glm::vec3 Position;
		glm::vec3 Velocity;
		glm::vec3 Color;
		float Size;
		float Lifetime;
		float Lifespan;
	};

	struct ParticleVertex
	{
		ParticleVertex()
			: Position(0.f)
			, Color(0.f)
		{

		}

		glm::vec3 Position;
		glm::vec3 Color;
		glm::vec3 Normal;
		glm::vec2 TexCoord;
	};

	class ParticleEmitter
	{
	public:

		ParticleEmitter();
		~ParticleEmitter();

	public:

		void Initialise(
			uint32 maxparticles, uint32 emitrate, float lifetimemin, float lifetimemax,
			float velocitymin, float velocitymax, float startsize, float endsize,
			const glm::vec3& startcolor, const glm::vec3& endcolor);

		void Update(float deltaTime, const glm::mat4& camtransform);

		void Draw();

		void Emit();

	public:

		inline uint32 Count() const { return m_Particles.size(); }

	protected:

		std::vector<Particle> m_Particles;
		uint32 m_FirstInactive;

		uint32 m_VAO, m_VBO, m_IBO;
		std::vector<ParticleVertex> m_VertexData;

	public:

		glm::vec3 m_Position;

	protected:

		float m_EmitTimer;
		float m_EmitRate;

		float m_LifespanMin;
		float m_LifespanMax;

		float m_VelocityMin;
		float m_VelocityMax;

		float m_StartSize;
		float m_EndSize;

		glm::vec3 m_StartColor;
		glm::vec3 m_EndColor;
	};
}