#include "Rendering\Shaders\ShaderUtility.h"

#include <fstream>

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
				fin.close();
				return false;
			}

			out_shader.assign(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>());

			fin.close();
			return true;
		}
	}
}

