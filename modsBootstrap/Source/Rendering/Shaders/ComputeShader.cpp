#include "Rendering\Shaders\ComputeShader.h"

#include "IncludeGLFW.h"
#include "Rendering\Shaders\ShaderUtility.h"

#include <glm\gtc\type_ptr.hpp>

#include <iostream>
#include <vector>

namespace mods
{
	ComputeShader::ComputeShader()
		: m_Program(0)
	{

	}

	ComputeShader::ComputeShader(const std::string& path)
		: m_Program(0)
	{
		Load(path);
	}

	ComputeShader::ComputeShader(ComputeShader&& rhs)
		: m_Program(rhs.m_Program)
		, m_Uniforms(std::move(rhs.m_Uniforms))
	{
		rhs.m_Program = 0;
	}

	ComputeShader::~ComputeShader()
	{
		Destroy();
	}

	ComputeShader& ComputeShader::operator=(ComputeShader&& rhs)
	{
		// Destroy existing program first
		Destroy();

		m_Program = rhs.m_Program;
		m_Uniforms = std::move(rhs.m_Uniforms);

		rhs.m_Program = 0;

		return *this;
	}

	bool ComputeShader::Load(const std::string& path)
	{
		// Load shader from source
		std::string compute;
		if (!detail::LoadShaderFromSource(path, compute))
			return false;

		return CreateProgram(compute, path);
	}

	bool ComputeShader::Create(const std::string& compute)
	{
		return CreateProgram(compute);
	}

	void ComputeShader::Destroy()
	{
		if (IsValid())
		{
			glDeleteProgram(m_Program);
			
			m_Program = 0;
			m_Uniforms.clear();
		}
	}

	void ComputeShader::Bind() const
	{
		glUseProgram(m_Program);
	}

	void ComputeShader::Unbind() const
	{
		glUseProgram(0);
	}

	void ComputeShader::Dispatch(uint32 x, uint32 y, uint32 z) const
	{
		glDispatchCompute(x, y, z);
	}

	void ComputeShader::SetUniformValue(const std::string& name, bool value)
	{
		SetUniformValue(name, (int32)value);
	}

	void ComputeShader::SetUniformValue(const std::string& name, int32 value)
	{
		int32 location;
		if (GetUniformLocation(name, location))
			glUniform1i(location, value);
	}

	void ComputeShader::SetUniformValue(const std::string& name, uint32 value)
	{
		int32 location;
		if (GetUniformLocation(name, location))
			glUniform1ui(location, value);
	}

	void ComputeShader::SetUniformValue(const std::string& name, float value)
	{
		int32 location;
		if (GetUniformLocation(name, location))
			glUniform1f(location, value);
	}

	void ComputeShader::SetUniformValue(const std::string& name, const glm::vec2& value)
	{
		int32 location;
		if (GetUniformLocation(name, location))
			glUniform2fv(location, 1, glm::value_ptr(value));
	}

	void ComputeShader::SetUniformValue(const std::string& name, const glm::vec3& value)
	{
		int32 location;
		if (GetUniformLocation(name, location))
			glUniform3fv(location, 1, glm::value_ptr(value));
	}

	void ComputeShader::SetUniformValue(const std::string& name, const glm::vec4& value)
	{
		int32 location;
		if (GetUniformLocation(name, location))
			glUniform4fv(location, 1, glm::value_ptr(value));
	}

	void ComputeShader::SetUniformValue(const std::string& name, const glm::mat3& value, bool transpose)
	{
		int32 location;
		if (GetUniformLocation(name, location))
			glUniformMatrix3fv(location, 1, transpose, glm::value_ptr(value));
	}

	void ComputeShader::SetUniformValue(const std::string& name, const glm::mat4& value, bool transpose)
	{
		int32 location;
		if (GetUniformLocation(name, location))
			glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(value));
	}

	bool ComputeShader::CreateProgram(const std::string& script, const std::string& path)
	{
		// Need to destroy existing program first
		Destroy();

		// Compile shader
		uint32 cshader;
		if (!detail::CreateAndCompileShader(script, GL_COMPUTE_SHADER, cshader, path))
			return false;

		std::vector<uint32> shader;
		shader.push_back(cshader);

		// Link shader into single program (destroying it aswell)
		return detail::CreateAndLinkProgram(shader, m_Program);
	}

	bool ComputeShader::GetUniformLocation(const std::string& name, int32& location)
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