#include "Rendering\Shader\Shader.h"

#include "IncludeGLFW.h"

#include <glm\gtc\type_ptr.hpp>

#include <fstream>
#include <iostream>

namespace mods
{
	mdShader::mdShader()
		: m_Program(InvalidProgram)
	{

	}

	mdShader::mdShader(mdShader&& rhs)
	{
		// Delete any existing program
		if (IsValid())
			glDeleteProgram(m_Program);

		m_Program = rhs.m_Program;
		m_UniformCache = rhs.m_UniformCache;

		// Nullify references from other program
		rhs.m_Program = InvalidProgram;
		rhs.m_UniformCache.clear();
	}

	mdShader::~mdShader()
	{
		Clear();
	}

	mdShader& mdShader::operator=(mdShader&& rhs)
	{
		// Delete any existing program
		if (IsValid())
			glDeleteProgram(m_Program);

		m_Program = rhs.m_Program;
		m_UniformCache = rhs.m_UniformCache;

		// Nullify references from other program
		rhs.m_Program = InvalidProgram;
		rhs.m_UniformCache.clear();

		return *this;
	}

	bool mdShader::Create(const std::string& vertex, const std::string& fragment)
	{
		// Clear any existing program
		Clear();

		// Load the scripts from the files
		std::string vsource, fsource;
		if (!LoadSource(vertex, vsource) || !LoadSource(fragment, fsource))
			return false;

		// Create the shaders, make sure they are valid
		unsigned int shaders[2];
		if (!CreateShader(vsource, mdShaderType::Vertex, shaders[0]))
			return false;
		else if (!CreateShader(fsource, mdShaderType::Fragment, shaders[1]))
		{
			glDeleteShader(shaders[0]);
			return false;
		}

		// Create the program, return if the action was successfull
		return CreateProgram(shaders, 2, m_Program);
	}

	void mdShader::Clear()
	{
		if (!IsValid())
			return;

		glDeleteProgram(m_Program);

		// Clear any references to prior program
		m_Program = InvalidProgram;
		m_UniformCache.clear();
	}

	void mdShader::Bind()
	{
		glUseProgram(m_Program);
	}

	void mdShader::Unbind()
	{
		glUseProgram(0);
	}

	void mdShader::SetUniformValue(const std::string& name, bool value)
	{
		SetUniformValue(name, (int)value);
	}

	void mdShader::SetUniformValue(const std::string& name, int value)
	{
		int location;
		if (GetUniformLocation(name, location))
			glUniform1i(location, value);
	}

	void mdShader::SetUniformValue(const std::string& name, float value)
	{
		int location;
		if (GetUniformLocation(name, location))
			glUniform1f(location, value);
	}

	void mdShader::SetUniformValue(const std::string& name, const glm::vec2& value)
	{
		int location;
		if (GetUniformLocation(name, location))
			glUniform2fv(location, 1, glm::value_ptr(value));
	}

	void mdShader::SetUniformValue(const std::string& name, const glm::vec3& value)
	{
		int location;
		if (GetUniformLocation(name, location))
			glUniform3fv(location, 1, glm::value_ptr(value));
	}

	void mdShader::SetUniformValue(const std::string& name, const glm::mat4& value, bool transpose)
	{
		int location;
		if (GetUniformLocation(name, location))
			glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(value));
	}

	bool mdShader::LoadSource(const std::string& path, std::string& script)
	{
		script.clear();

		// Only continue if good to read from
		std::ifstream fin(path, std::ios::in);
		if (!fin.good())
		{
			std::cout << "Error: Failed to open shader source file at path: " << path << std::endl;

			// Always close the file
			fin.close();
			return false;
		}

		script.assign(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>());

		// Always close the file
		fin.close();
		return true;
	}

	bool mdShader::CreateShader(const std::string& source, mdShaderType type, unsigned int& shader)
	{
		const char* script = source.c_str();

		// Create and compile the script
		shader = glCreateShader((unsigned int)type);
		glShaderSource(shader, 1, &script, nullptr);
		glCompileShader(shader);

		// Handle any potential errors
		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char log[512];
			glGetShaderInfoLog(shader, 512, nullptr, log);

			std::cout << "Error: Failed to compile shader. Log:\n" << log << std::endl;
			return false;
		}

		return true;
	}

	bool mdShader::CreateProgram(unsigned int* const shaders, int count, unsigned int& program)
	{
		program = glCreateProgram();

		// Attach all shaders to this program
		for (int i = 0; i < count; ++i)
			glAttachShader(program, shaders[i]);

		glLinkProgram(program);

		// Handle any potential errors
		int success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			char log[512];
			glGetShaderInfoLog(program, 512, nullptr, log);

			std::cout << "Error: Failed to link program. Log:\n" << log << std::endl;
		
			// Delete failed program and shaders
			glDeleteProgram(program);
			for (int i = 0; i < count; ++i)
			{
				glDeleteShader(count);
			}

			return false;
		}
		else
		{
			// Detach and destroy the shaders
			for (int i = 0; i < count; ++i)
			{
				glDetachShader(program, count);
				glDeleteShader(count);
			}

			return true;
		}
	}

	bool mdShader::GetUniformLocation(const std::string& name, int& location)
	{
		location = -1;

		// Need a valid program before we can get a location
		if (!IsValid())
		{
			std::cout << "Error: Can't retrieve uniform location with invalid program.\nUniform name = " << name << std::endl;
			return false;
		}

		// Check if location has already been indexed
		auto it = m_UniformCache.find(name);
		if (it != m_UniformCache.cend())
		{
			location = it->second;
			return true;
		}		

		// Make certain this uniform exists
		location = glGetUniformLocation(m_Program, name.c_str());
		if (location == -1)
		{
			std::cout << "Error: Location of uniform " << name << " was not found and might exist" << std::endl;
			return false;
		}

		m_UniformCache[name] = location;
		return true;
	}
}