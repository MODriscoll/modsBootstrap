#pragma once

#include "ShaderTypes.h"

#include <glm\fwd.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace mods
{
	// Context for a new shader to create
	struct ShaderContext final
	{
		friend class Shader;

	private:

		struct Data
		{
			// Type of shader this is
			eShaderType Type;

			// Path to shaders source
			std::string Path;
		};

	public:

		// Adds a new shader to create
		void AddShader(const std::string& path, eShaderType type);

	private:

		// All shaders to link into a shader program
		std::vector<Data> m_Shaders;
	};

	// Wraps the handle to a shader program
	class Shader
	{
	public:

		Shader();
		Shader(const Shader& rhs) = delete;
		Shader(Shader&& rhs);

		virtual ~Shader();

		Shader& operator = (const Shader& rhs) = delete;
		Shader& operator = (Shader&& rhs);

	public:

		// Loads and compiles a new shader program.
		// This function expects the file paths for the shaders scripts
		virtual bool Create(const std::string& vertex, const std::string& fragment);
		
		// Deletes program and invalidates this shader
		virtual void Clear();

		// Binds this shader for use
		virtual void Bind();

		// Unbinds this shader from use
		virtual void Unbind();

	public:

		// All supported uniforms types as of now
		void SetUniformValue(const std::string& name, bool value);
		void SetUniformValue(const std::string& name, int value);
		void SetUniformValue(const std::string& name, float value);		
		void SetUniformValue(const std::string& name, const glm::vec2& value);
		void SetUniformValue(const std::string& name, const glm::vec3& value);
		void SetUniformValue(const std::string& name, const glm::mat4& value, bool transpose = false);

	public:

		// Get if this shader has a valid program
		inline bool IsValid() const { return m_Program != InvalidProgram; }

	protected:
		
		// Loads a shader script from the file at given path
		virtual bool LoadSource(const std::string& path, std::string& script);

		// Creates and compiles a new shader from the given script
		virtual bool CreateShader(const std::string& source, eShaderType type, unsigned int& shader);

		// Creates and compiles a new program from the given shaders
		virtual bool CreateProgram(unsigned int* const shaders, int count, unsigned int& program);

		// Get the location of the specified uniform
		bool GetUniformLocation(const std::string& name, int& location);

	protected:

		// Handle to the shader program
		unsigned int m_Program;

		// Locations of all uniform values that have been set
		std::unordered_map<std::string, int> m_UniformCache;
	};
}