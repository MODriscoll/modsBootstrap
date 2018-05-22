#include "Rendering\Shaders\Shader.h"

#include "IncludeGLFW.h"

#include <glm\gtc\type_ptr.hpp>

#include <fstream>
#include <iostream>

namespace mods
{
	void ShaderProgramConstructor::LoadShader(eShaderType type, const std::string& path)
	{
		byte bit = GetBitForType(type);

		// Don't overwrite the existing compiled shader
		if ((m_Mask & bit) == bit)
		{		
			std::cout << "Warning: " << GetStringForType(type) << " has already been set." << std::endl;
			return;
		}

		// Attempt to load the source from the given path
		std::string source;
		if (!LoadSource(path, source))
			return;

		// Compile the source into a shader
		uint32 shader;
		if (!CompileSource(type, source, shader))
		{
			glDeleteShader(shader);
			return;
		}

		m_Mask |= bit;
		m_Shaders.push_back(shader);
	}

	void ShaderProgramConstructor::LoadShader(const ShaderSource& source)
	{
		LoadShader(source.Type, source.Path);
	}

	std::shared_ptr<ShaderProgram> ShaderProgramConstructor::Construct()
	{
		// A shader requires at least a
		// vertex and a fragment shader
		if ((m_Mask & Required) != Required)
		{
			std::cout << "Error: Shader program constructor requires both "
				"a vertex and a fragment shader in order to construct.\n\nMissing:";

			if ((m_Mask & Vertex) == 0)
				std::cout << "Vertex\n";
			if ((m_Mask & Fragment) == 0)
				std::cout << "Fragment";

			std::cout << std::endl;

			return std::shared_ptr<ShaderProgram>(new ShaderProgram(0));
		}

		uint32 program = glCreateProgram();

		for (uint32 shader : m_Shaders)
			glAttachShader(program, shader);

		glLinkProgram(program);

		// Handle potential errors
		int32 success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			char log[512];
			glGetShaderInfoLog(program, 512, nullptr, log);

			std::cout << "Error: Failed to link program. Log:\n" << log << std::endl;

			// Delete failed program and shaders
			glDeleteProgram(program);
			for (uint32 shader : m_Shaders)
				glDeleteShader(shader);

			return std::shared_ptr<ShaderProgram>(new ShaderProgram(0));
		}
		else
		{
			// Delete only the shaders
			for (uint32 shader : m_Shaders)
			{
				glDetachShader(program, shader);
				glDeleteShader(shader);
			}

			return std::shared_ptr<ShaderProgram>(new ShaderProgram(program));
		}
	}

	bool ShaderProgramConstructor::LoadSource(const std::string& path, std::string& source) const
	{
		// Only continue if good to read from
		std::ifstream fin(path, std::ios::in);
		if (!fin.good())
		{
			std::cout << "Error: Failed to open shader source file at path: " << path << std::endl;

			// Always close the file
			fin.close();
			return false;
		}

		source.assign(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>());

		// Always close the file
		fin.close();
		return true;
	}

	bool ShaderProgramConstructor::CompileSource(eShaderType type, const std::string& source, uint32& shader) const
	{
		const char* script = source.c_str();

		// Create and compile the script
		shader = glCreateShader((uint32)type);
		glShaderSource(shader, 1, &script, nullptr);
		glCompileShader(shader);

		// Handle any potential errors
		int32 success;
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

	constexpr ShaderProgramConstructor::eShaderMask ShaderProgramConstructor::GetBitForType(eShaderType type) const
	{
		switch (type)
		{
			case eShaderType::Vertex:			{ return Vertex; }
			case eShaderType::TessControl:		{ return TessControl; }
			case eShaderType::TessEvaluation:	{ return TessEveluation; }
			case eShaderType::Geometry:			{ return Geometry; }
			case eShaderType::Fragment:			{ return Fragment; }
		}

		assert(false);
		return Vertex;
	}

	constexpr const char* ShaderProgramConstructor::GetStringForType(eShaderType type) const
	{
		switch (type)
		{
			case eShaderType::Vertex:			{ return "Vertex"; }
			case eShaderType::TessControl:		{ return "TessControl"; }
			case eShaderType::TessEvaluation:	{ return "TessEveluation"; }
			case eShaderType::Geometry:			{ return "Geometry"; }
			case eShaderType::Fragment:			{ return "Fragment"; }
		}

		assert(false);
		return "Invalid";
	}

	ShaderProgram::ShaderProgram(uint32 program)
		: m_Program(program)
	{

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

	void ShaderProgram::Bind()
	{
		glUseProgram(m_Program);
	}

	void ShaderProgram::Unbind()
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