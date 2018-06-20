#include "Rendering\Shaders\ShaderUtility.h"

#include "IncludeGLFW.h"

#include <fstream>
#include <iostream>

namespace mods
{
	namespace detail
	{
		bool LoadShaderFromSource(const std::string& path, std::string& out_shader)
		{
			out_shader.clear();

			// Only continue if good to read from
			std::ifstream fin(path, std::ios::in);
			if (!fin.good())
			{
				std::cout << "Error: Failed to open shader source\nPath: " << path << std::endl;

				fin.close();
				return false;
			}

			// Substring to find for include directives
			constexpr auto include_directive = "#include";
			// Length of include directive with one additional offset
			constexpr auto include_length_offset = 10;

			for (std::string line; std::getline(fin, line); )
			{
				// Replace this line with a new shader source if requested
				if (line.find(include_directive) == 0)
				{
					std::string include = line.substr(include_length_offset, line.length() - include_length_offset - 1);

					std::string shader;
					if (!LoadShaderFromSource(include, shader))
					{
						std::cout << "Error: Failed to open include directive\nInclude Path: " << include << "\nSource Path: " << path << std::endl;

						fin.close();
						return false;
					}

					// Only include the newly added shader source,
					// we no longer need to include directive
					out_shader.append(shader);
				}
				else
				{
					// Keeps formatting even if line is empty
					out_shader.append(line + '\n');
				}
			}

			fin.close();
			return true;
		}

		bool CreateAndCompileShader(const std::string& script, uint32 type, uint32& shader, const std::string& path)
		{
			const char* sc = script.c_str();

			shader = glCreateShader(type);
			glShaderSource(shader, 1, &sc, nullptr);
			glCompileShader(shader);

			// Handle compilation error if any
			int32 success;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				int32 length = 512;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

				std::vector<char> log(length, '?');
				glGetShaderInfoLog(shader, length, nullptr, log.data());

				std::cout << "Error: Failed to compile shader\nLog:\n" << log.data() << std::endl;
				if (!path.empty())
					std::cout << "\nPath: " << path << std::endl;

				// Destroy failed shader
				glDeleteShader(shader);

				shader = 0;
				return false;
			}

			return true;
		}

		bool CreateAndLinkProgram(const std::vector<uint32>& shaders, uint32& program, bool bDestroy)
		{
			if (shaders.empty())
			{
				std::cout << "Error: Cannot link program with no shaders" << std::endl;

				program = 0;
				return false;
			}

			program = glCreateProgram();

			// Attach first before linking
			for (uint32 shader : shaders)
				glAttachShader(program, shader);

			glLinkProgram(program);

			// Detach even on failure
			for (uint32 shader : shaders)
				glDetachShader(program, shader);

			// Handle linkage error if any
			int32 success;
			glGetProgramiv(program, GL_LINK_STATUS, &success);
			if (!success)
			{
				int32 length = 512;
				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

				std::vector<char> log(length, '?');
				glGetProgramInfoLog(program, length, nullptr, log.data());

				std::cout << "Error: Failed to link program\nLog:\n" << log.data() << std::endl;

				// Destroy the failed program
				glDeleteProgram(program);
				program = 0;
			}

			if (bDestroy)
			{
				for (uint32 shader : shaders)
					glDeleteShader(shader);
			}

			return program != 0;
		}
	}
}

