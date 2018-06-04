#include "Rendering\Shaders\Shader.h"

#include "IncludeGLFW.h"
#include "Rendering\Shaders\ShaderUtility.h"

#include <glm\gtc\type_ptr.hpp>

#include <iostream>

namespace mods
{
	ShaderProgram::ShaderProgram()
		: m_Program(0)
	{
	}

	ShaderProgram::ShaderProgram(const std::string& vpath, const std::string& fpath)
		: m_Program(0)
	{
		Load(vpath, fpath);
	}

	ShaderProgram::ShaderProgram(const std::string& vpath, const std::string& gpath, const std::string& fpath)
		: m_Program(0)
	{
		Load(vpath, gpath, fpath);
	}

	ShaderProgram::ShaderProgram(ShaderProgram&& rhs)
		: m_Program(rhs.m_Program)
		, m_Uniforms(std::move(rhs.m_Uniforms))
	{
		rhs.m_Program = 0;
	}

	ShaderProgram::~ShaderProgram()
	{
		if (IsValid())
			glDeleteProgram(m_Program);
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& rhs)
	{
		// Need to delete the existing program
		if (IsValid())
			glDeleteProgram(m_Program);

		m_Program = rhs.m_Program;
		m_Uniforms = std::move(rhs.m_Uniforms);

		rhs.m_Program = 0;

		return *this;
	}

	bool ShaderProgram::Load(const std::string& vpath, const std::string& fpath)
	{
		std::string vertex, fragment;
		if (!detail::LoadShaderFromSource(vpath, vertex) ||
			!detail::LoadShaderFromSource(fpath, fragment))
		{
			return false;
		}

		return Create(vertex, fragment);
	}

	bool ShaderProgram::Load(const std::string& vpath, const std::string& gpath, const std::string& fpath)
	{
		std::string vertex, geometry, fragment;
		if (!detail::LoadShaderFromSource(vpath, vertex) ||
			!detail::LoadShaderFromSource(gpath, geometry) ||
			!detail::LoadShaderFromSource(fpath, fragment))
		{
			return false;
		}

		return Create(vertex, geometry, fragment);
	}

	bool ShaderProgram::Create(const std::string& vertex, const std::string& fragment)
	{
		uint32 vshader;
		if (!CompileShader(vertex, GL_VERTEX_SHADER, vshader))
			return false;

		uint32 fshader;
		if (!CompileShader(fragment, GL_FRAGMENT_SHADER, fshader))
		{
			glDeleteShader(vshader);
			return false;
		}

		uint32 program;
		if (!LinkShaders(vshader, 0, fshader, program))
			return false;

		m_Program = program;
		return true;
	}

	bool ShaderProgram::Create(const std::string& vertex, const std::string& geometry, const std::string& fragment)
	{
		uint32 vshader;
		if (!CompileShader(vertex, GL_VERTEX_SHADER, vshader))
			return false;

		uint32 gshader;
		if (!CompileShader(geometry, GL_GEOMETRY_SHADER, gshader))
		{
			glDeleteShader(vshader);
			return false;
		}

		uint32 fshader;
		if (!CompileShader(fragment, GL_FRAGMENT_SHADER, fshader))
		{
			glDeleteShader(vshader);
			glDeleteShader(gshader);
			return false;
		}

		uint32 program;
		if (!LinkShaders(vshader, gshader, fshader, program))
			return false;

		m_Program = program;
		return true;
	}

	void ShaderProgram::Bind() const
	{
		glUseProgram(m_Program);
	}

	void ShaderProgram::Unbind() const
	{
		glUseProgram(0);
	}

	void ShaderProgram::SetUniformValue(const std::string& name, bool value)
	{
		SetUniformValue(name, (int32)value);
	}

	void ShaderProgram::SetUniformValue(const std::string& name, int32 value)
	{
		int32 location;
		if (GetUniformLocation(name, location))
			glUniform1i(location, value);
	}

	void ShaderProgram::SetUniformValue(const std::string& name, float value)
	{
		int32 location;
		if (GetUniformLocation(name, location))
			glUniform1f(location, value);
	}

	void ShaderProgram::SetUniformValue(const std::string& name, const glm::vec2& value)
	{
		int32 location;
		if (GetUniformLocation(name, location))
			glUniform2fv(location, 1, glm::value_ptr(value));
	}

	void ShaderProgram::SetUniformValue(const std::string& name, const glm::vec3& value)
	{
		int32 location;
		if (GetUniformLocation(name, location))
			glUniform3fv(location, 1, glm::value_ptr(value));
	}

	void ShaderProgram::SetUniformValue(const std::string& name, const glm::vec4& value)
	{
		int32 location;
		if (GetUniformLocation(name, location))
			glUniform4fv(location, 1, glm::value_ptr(value));
	}

	void ShaderProgram::SetUniformValue(const std::string& name, const glm::mat3& value, bool transpose)
	{
		int32 location;
		if (GetUniformLocation(name, location))
			glUniformMatrix3fv(location, 1, transpose, glm::value_ptr(value));
	}

	void ShaderProgram::SetUniformValue(const std::string& name, const glm::mat4& value, bool transpose)
	{
		int32 location;
		if (GetUniformLocation(name, location))
			glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(value));
	}

	bool ShaderProgram::CompileShader(const std::string& script, uint32 type, uint32& shader)
	{
		const char* sc = script.c_str();

		shader = glCreateShader(type);
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

			return false;
		}

		return true;
	}

	bool ShaderProgram::LinkShaders(uint32 vertex, uint32 geometry, uint32 fragment, uint32& program)
	{
		program = glCreateProgram();

		glAttachShader(program, vertex);
		if (geometry > 0)
			glAttachShader(program, geometry);
		glAttachShader(program, fragment);

		glLinkProgram(program);

		// Handle potential errors
		int32 success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			char log[512];
			glGetProgramInfoLog(program, 512, nullptr, log);

			std::cout << "Error: Failed to link program. Log:\n" << log << std::endl;

			// Delete failed program and shaders
			glDeleteProgram(program);
			glDeleteShader(vertex);
			if (geometry > 0)
				glDeleteShader(geometry);
			glDeleteShader(fragment);

			return false;
		}
	
		glDetachShader(program, vertex);
		if (geometry > 0)
			glDetachShader(program, geometry);
		glDetachShader(program, fragment);

		glDeleteShader(vertex);
		if (geometry > 0)
			glDeleteShader(geometry);
		glDeleteShader(fragment);

		return true;
	}

	bool ShaderProgram::GetUniformLocation(const std::string& name, int32& location)
	{
		location = -1;

		// Check if location has already been indexed
		auto it = m_Uniforms.find(name);
		if (it != m_Uniforms.cend())
		{
			location = it->second;
			return true;
		}

		// Make certain this uniform exists
		location = glGetUniformLocation(m_Program, name.c_str());
		if (location == -1)
		{
			//std::cout << "Error: Location of uniform " << name << " was not found." << std::endl;
			return false;
		}

		m_Uniforms[name] = location;
		return true;
	}
}