#include "Rendering\Meshes\Model.h"

#include "Rendering\Meshes\MeshUtility.h"

namespace mods
{
	Model::Model(const std::string& path, eProcessModel process)
	{
		Load(path, process);
	}

	bool Model::Load(const std::string& path, eProcessModel process)
	{
		detail::ModelData data;
		if (!detail::LoadModelFromSource(path, data, process))
			return false;
		
		m_Meshes = std::move(data.Meshes);
		m_Directory = path;

		return true;
	}

	void Model::Draw(ShaderProgram& program) const
	{
		for (const Mesh& mesh : m_Meshes)
			mesh.Draw(program);
	}
}