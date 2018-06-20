#include "Rendering\Shaders\ShaderUtility.h"

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
				std::cout << "Error: Failed to open shader source - Path: " << path << std::endl;

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
						std::cout << "Error: Failed to open include directive - Include Path: " << include << " in shader source - Path: " << path << std::endl;

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
	}
}

