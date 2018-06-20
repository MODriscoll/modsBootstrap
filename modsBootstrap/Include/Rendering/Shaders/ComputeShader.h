#pragma once

#include "Types.h"

#include <glm\fwd.hpp>

#include <string>
#include <unordered_map>

namespace mods
{
	class ComputeShader
	{
	public:

		ComputeShader();
		ComputeShader(const std::string& path);
		ComputeShader(const ComputeShader& rhs) = delete;
		ComputeShader(ComputeShader&& rhs);

		~ComputeShader();

		ComputeShader& operator = (const ComputeShader& rhs) = delete;
		ComputeShader& operator = (ComputeShader&& rhs);

	public:

		// Loads a new program from given path
		bool Load(const std::string& path);

		// Creates a new program from given shader
		bool Create(const std::string& compute);

		// Destroys existing shader if any
		void Destroy();

	public:

		// Binds this shader for use
		void Bind() const;

		// Unbinds this shader for use
		void Unbind() const;

		// Dispatches this compute shader with given amount of work groups
		void Dispatch(uint32 x, uint32 y, uint32 z) const;

	public:

		inline bool IsValid() const { return m_Program != 0; }

	public:

		// Sets the uniform with the given name to the given value
		void SetUniformValue(const std::string& name, bool value);
		void SetUniformValue(const std::string& name, int32 value);
		void SetUniformValue(const std::string& name, uint32 value);
		void SetUniformValue(const std::string& name, float value);
		void SetUniformValue(const std::string& name, const glm::vec2& value);
		void SetUniformValue(const std::string& name, const glm::vec3& value);
		void SetUniformValue(const std::string& name, const glm::vec4& value);
		void SetUniformValue(const std::string& name, const glm::mat3& value, bool transpose = false);
		void SetUniformValue(const std::string& name, const glm::mat4& value, bool transpose = false);

	private:

		// Creates and links the shader program
		// Path is used to print source of error if linkage fails
		bool CreateProgram(const std::string& script, const std::string& path = std::string());

		// Retrieves the location of the given uniform.
		// Saves location for later indexing if uniform exists
		bool GetUniformLocation(const std::string& name, int32& location);

	private:

		// Handle to the program
		uint32 m_Program;

		// Locations for the uniform values that have been set
		std::unordered_map<std::string, int32> m_Uniforms;
	};
}