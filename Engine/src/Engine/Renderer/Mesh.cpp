#include "egpch.h"
#include "Mesh.h"

#include <set>

namespace Engine {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Mesh ///////////////////////////////////////////////////////////////////////////////////////

	Mesh::Mesh(const std::string& name, const std::string& path, std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& textureCoordinates, std::vector<Submesh>& submeshes)
		: m_Name(name), m_Path(path), m_Positions(positions), m_Normals(normals), m_TextureCoordinates(textureCoordinates), m_Submeshes(submeshes)
	{
		m_GlMesh = CreateGlMesh(true, true, true, {
			{ OpenGL::GlShaderDataType::Float3, "a_Position" },
			{ OpenGL::GlShaderDataType::Float2, "a_TexCoord" },
			{ OpenGL::GlShaderDataType::Float3, "a_Normals" }
		});

		m_PxMesh = CreatePsMesh();
	}

	Ref<OpenGL::GlMesh> Mesh::CreateGlMesh(bool positions, bool texcoords, bool normals, OpenGL::GlVertexBufferLayout layout)
	{
		std::set<std::vector<uint32_t>> submeshIndices;

		std::vector<float> vertices;
		std::vector<uint32_t> indices;
		uint32_t indexCount = 0;
		for (auto& submeshe : m_Submeshes)
		{
			for (auto& face : submeshe.Faces)
			{
				for (uint16_t v = 0; v < face.vertices; v++)
				{
					const glm::vec3& p = m_Positions[face.positionIndex[v]];
					const glm::vec2& t = m_TextureCoordinates[face.textureCoordinateIndex[v]];
					const glm::vec3& n = m_Normals[face.normalIndex[v]];

					if (positions)
					{
						vertices.push_back(p.x);
						vertices.push_back(p.y);
						vertices.push_back(p.z);
					}
					if (texcoords)
					{
						vertices.push_back(t.x);
						vertices.push_back(t.y);
					}
					if (normals)
					{
						vertices.push_back(n.x);
						vertices.push_back(n.y);
						vertices.push_back(n.z);
					}

					indices.push_back(indexCount++);
				}
			}
			submeshIndices.insert(indices);
			indices.clear();
		}

		Ref<OpenGL::GlMesh> mesh = CreateRef<OpenGL::GlMesh>(m_Name, vertices, layout);

		for (auto sm : submeshIndices)
			mesh->AddSubmesh(sm);

		return mesh;
	}

	Ref<Physics::PsMesh> Mesh::CreatePsMesh()
	{
		std::vector<uint32_t> indices;
		for (auto& submeshe : m_Submeshes)
			for (auto& face : submeshe.Faces)
				for (uint16_t v = 0; v < face.vertices; v++)
					indices.push_back(face.positionIndex[v]);

		return CreateRef<Physics::PsMesh>(m_Positions, indices);
	}

	

}