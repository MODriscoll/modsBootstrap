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
		// Load shaders from files
		std::string vertex, fragment;
		if (!detail::LoadShaderFromSource(vpath, vertex) ||
			!detail::LoadShaderFromSource(fpath, fragment))
		{
			return false;
		}

		// Compile shaders
		uint32 vshader;
		if (!detail::CreateAndCompileShader(vertex, GL_VERTEX_SHADER, vshader, vpath))
			return false;

		uint32 fshader;
		if (!detail::CreateAndCompileShader(fragment, GL_FRAGMENT_SHADER, fshader, fpath))
		{
			glDeleteShader(vshader);
			return false;
		}

		std::vector<uint32> shaders;
		shaders.push_back(vshader);
		shaders.push_back(fshader);

		// Link shaders into single program (destroying them aswell)
		return detail::CreateAndLinkProgram(shaders, m_Program);
	}

	bool ShaderProgram::Load(const std::string& vpath, const std::string& gpath, const std::string& fpath)
	{
		// Load shaders from files
		std::string vertex, geometry, fragment;
		if (!detail::LoadShaderFromSource(vpath, vertex) ||
			!detail::LoadShaderFromSource(gpath, geometry) ||
			!detail::LoadShaderFromSource(fpath, fragment))
		{
			return false;
		}

		// Compile shaders
		uint32 vshader;
		if (!detail::CreateAndCompileShader(vertex, GL_VERTEX_SHADER, vshader, vpath))
			return false;

		uint32 gshader;
		if (!detail::CreateAndCompileShader(geometry, GL_GEOMETRY_SHADER, gshader, gpath))
		{
			glDeleteShader(vshader);
			return false;
		}

		uint32 fshader;
		if (!detail::CreateAndCompileShader(fragment, GL_FRAGMENT_SHADER, fshader, fpath))
		{
			glDeleteProgram(vshader);
			glDeleteShader(gshader);
			return false;
		}

		std::vector<uint32> shaders;
		shaders.push_back(vshader);
		shaders.push_back(gshader);
		shaders.push_back(fshader);

		// Link shaders into single program (destroying them aswell)
		return detail::CreateAndLinkProgram(shaders, m_Program);
	}

	bool ShaderProgram::Create(const std::string& vertex, const std::string& fragment)
	{
		// Compile shaders
		uint32 vshader;
		if (!detail::CreateAndCompileShader(vertex, GL_VERTEX_SHADER, vshader))
			return false;

		uint32 fshader;
		if (!detail::CreateAndCompileShader(fragment, GL_FRAGMENT_SHADER, fshader))
		{
			glDeleteShader(vshader);
			return false;
		}

		std::vector<uint32> shaders;
		shaders.push_back(vshader);
		shaders.push_back(fshader);

		// Link shaders into single program (destroying them aswell)
		return detail::CreateAndLinkProgram(shaders, m_Program);
	}

	bool ShaderProgram::Create(const std::string& vertex, const std::string& geometry, const std::string& fragment)
	{
		// Compile shaders
		uint32 vshader;
		if (!detail::CreateAndCompileShader(vertex, GL_VERTEX_SHADER, vshader))
			return false;

		uint32 gshader;
		if (!detail::CreateAndCompileShader(geometry, GL_GEOMETRY_SHADER, gshader))
		{
			glDeleteShader(vshader);
			return false;
		}

		uint32 fshader;
		if (!detail::CreateAndCompileShader(fragment, GL_FRAGMENT_SHADER, fshader))
		{
			glDeleteProgram(vshader);
			glDeleteShader(gshader);
			return false;
		}

		std::vector<uint32> shaders;
		shaders.push_back(vshader);
		shaders.push_back(gshader);
		shaders.push_back(fshader);

		// Link shaders into single program (destroying them aswell)
		return detail::CreateAndLinkProgram(shaders, m_Program);
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