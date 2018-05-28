#include "Rendering\Meshes\Model.h"

#include "Rendering\Meshes\MeshUtility.h"

namespace mods
{
	Model::Model(const std::string& path)
	{
		LoadModel(path);
	}

	void Model::Draw(ShaderProgram& program) const
	{
		for (const Mesh& mesh : m_Meshes)
			mesh.Draw(program);
	}

	void Model::LoadModel(const std::string& path)
	{
		detail::ModelData data;
		if (detail::LoadModelFromSource(path, data, aiProcess_Triangulate))
		{
			m_Meshes = std::move(data.Meshes);
			m_Directory = path;
		}
	}
}