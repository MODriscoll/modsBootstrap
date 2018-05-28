#pragma once

#include "Types.h"

#include "Rendering\Materials\Material.h"

#include <glm\vec2.hpp>
#include <glm\vec3.hpp>

#include <vector>

namespace mods
{
	class ShaderProgram;

	struct MeshVertex
	{
	public:

		MeshVertex()
			: Position(0.f)
			, Normal(0.f)
			, TexCoords(0.f)
		{

		}

	public:

		// Position of vertex
		glm::vec3 Position;

		// Normal of vertex
		glm::vec3 Normal;

		// Tex Coords of vertex
		glm::vec2 TexCoords;
	};

	// TODO: Static mesh and dynamic mesh
	// TODO: Mesh constructor should move its contents to the mesh, for now
	// Contains the vertex array for a mesh
	class Mesh
	{
	//private:
	public:

		Mesh(
			const std::vector<MeshVertex>& vertices, 
			const std::vector<uint32>& indices, 
			const Material& material);

	public:

		Mesh(const Mesh& rhs) = delete;
		Mesh(Mesh&& rhs);

		virtual ~Mesh();

		Mesh& operator = (const Mesh& rhs) = delete;
		Mesh& operator = (Mesh&& rhs);

	public:

		// Draw the mesh using given shader program
		virtual void Draw(ShaderProgram& program) const;

	protected:

		// Generates the vertex array handle
		// and binds all vertex attributes
		virtual void GenerateHandle();

	protected:

		// Handles to vertex and index buffer
		uint32 m_VBO, m_IBO;

		// Handle to vertex array object
		uint32 m_VAO;

		// This meshes material
		Material m_Material;

		// TODO: keep?
		std::vector<MeshVertex> m_Vertices;
		std::vector<uint32> m_Indices;
	};
}