#pragma once

#include "Types.h"

#include <glm\vec3.hpp>

#include <vector>

namespace mods
{
	class ShaderProgram;

	class GPUParticleSystem
	{
	public:
		
		~GPUParticleSystem();

	public:

		void Init(int32 MaxParticles);

		void Update();

		void Draw(ShaderProgram& program);

	public:

		glm::vec3 m_EmitterPosition = glm::vec3(0.f);

	protected:

		uint32 m_VAO;

		uint32 m_SSBs[3];

		std::vector<glm::vec4> m_Positions;
		std::vector<glm::vec4> m_Velocities;
		std::vector<glm::vec4> m_Colors;

		int32 m_Particles;

		uint32 m_Program;
	};
}