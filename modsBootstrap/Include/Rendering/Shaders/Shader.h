#pragma once

#include "Types.h"

#include <glm\fwd.hpp>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace mods
{
	enum class eShaderType : uint32
	{
		Vertex			= 0x8B31, 
		TessControl		= 0x8E88, 
		TessEvaluation	= 0x8E87,
		Geometry		= 0x8DD9,
		Fragment		= 0x8B30
	};

	// Wrapper for a shader program
	class ShaderProgram
	{
	public:

		ShaderProgram();
		ShaderProgram(const std::string& vertexpath, const std::string& fragmentpath);
		ShaderProgram(const std::string& vertexpath, const std::string& geometrypath, const std::string& fragmentpath);
		ShaderProgram(const ShaderProgram& rhs) = delete;
		ShaderProgram(ShaderProgram&& rhs);

		~ShaderProgram();

		ShaderProgram& operator = (const ShaderProgram& rhs) = delete;
		ShaderProgram& operator = (ShaderProgram&& rhs);

	public:

		// Loads a new program from the given paths
		bool Load(const std::string& vertexpath, const std::string& fragmentpath);
		bool Load(const std::string& vertexpath, const std::string& geometrypath, const std::string& fragmentpath);

		// Creates a new program from given shaders
		bool Create(const std::string& vertex, const std::string& fragment);
		bool Create(const std::string& vertex, const std::string& geometry, const std::string& fragment);

		// Binds this program for use
		void Bind() const;

		// Unbinds this program for use
		void Unbind() const;

	public:

		inline bool IsValid() const { return m_Program != 0; }

	public:

		// Sets the uniform with the given name to the given value
		void SetUniformValue(const std::string& name, bool value);
		void SetUniformValue(const std::string& name, int32 value);
		void SetUniformValue(const std::string& name, float value);
		void SetUniformValue(const std::string& name, const glm::vec2& value);
		void SetUniformValue(const std::string& name, const glm::vec3& value);
		void SetUniformValue(const std::string& name, const glm::vec4& value);
		void SetUniformValue(const std::string& name, const glm::mat3& value, bool transpose = false);
		void SetUniformValue(const std::string& name, const glm::mat4& value, bool transpose = false);

	private:

		// Compiles the script into a shader
		bool CompileShader(const std::string& script, uint32 type, uint32& shader);

		// Links the shaders into a single program
		bool LinkShaders(uint32 vertex, uint32 geometry, uint32 fragment, uint32& program);

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