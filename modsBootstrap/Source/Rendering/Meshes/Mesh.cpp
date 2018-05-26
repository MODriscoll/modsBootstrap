#include "Rendering\Meshes\Mesh.h"

#include "Rendering\Shaders\Shader.h"

#include "IncludeGLFW.h"

namespace mods
{
	Mesh::Mesh(
		const std::vector<MeshVertex>& vertices, 
		const std::vector<uint32>& indices, 
		Material&& material)
		: m_VBO(0)
		, m_IBO(0)
		, m_VAO(0)
		, m_Vertices(vertices)
		, m_Indices(indices)
		, m_Material(std::move(material))
	{
		m_Vertices.shrink_to_fit();
		m_Indices.shrink_to_fit();

		GenerateHandle();
	}

	Mesh::Mesh(Mesh&& rhs)
		: m_VBO(rhs.m_VBO)
		, m_IBO(rhs.m_IBO)
		, m_VAO(rhs.m_VAO)
		, m_Vertices(std::move(rhs.m_Vertices))
		, m_Indices(std::move(m_Indices))
		, m_Material(std::move(rhs.m_Material))
	{
		rhs.m_VBO = 0;
		rhs.m_IBO = 0;
		rhs.m_VAO = 0;
	}

	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_IBO);
	}

	Mesh& Mesh::operator=(Mesh&& rhs)
	{
		// Need to delete existing mesh
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_IBO);

		m_VBO = rhs.m_VBO;
		m_IBO = rhs.m_IBO;
		m_VAO = rhs.m_VAO;
		m_Vertices = std::move(rhs.m_Vertices);
		m_Indices = std::move(rhs.m_Indices);
		m_Material = std::move(rhs.m_Material);

		rhs.m_VBO = 0;
		rhs.m_IBO = 0;
		rhs.m_VAO = 0;

		return *this;
	}

	void Mesh::Draw(ShaderProgram& program) const
	{
		m_Material.Bind(program);
		
		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
	}

	void Mesh::GenerateHandle()
	{
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_IBO);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

		glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(MeshVertex), m_Vertices.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(uint32), m_Indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, Position));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, Normal));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, TexCoords));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}