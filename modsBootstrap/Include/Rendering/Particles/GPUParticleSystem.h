#pragma once

#include "Rendering\Shaders\ComputeShader.h"

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

		void Update(float deltaTime);

		void Draw(ShaderProgram& program);

	public:

		glm::vec3 m_EmitterPosition = glm::vec3(0.f);

	protected:

		uint32 m_VAO;

		// Position, velocity, color, size, age
		uint32 m_SSBs[5];

		// Dead list ssbos
		uint32 m_DeadList;

		float m_EmitTime = 0.f;
		float m_EmitRate;

		int32 m_Particles;

		uint32 m_DeadBuffer;

		ComputeShader m_EmitShader;
		ComputeShader m_ComputeShader;
	};
}