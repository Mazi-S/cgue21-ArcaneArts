#include "egpch.h"

#include <set>

#include "Mesh.h"
#include "Engine/Util/ObjectLoader.h"

namespace Engine {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Mesh ///////////////////////////////////////////////////////////////////////////////////////

	Mesh::Mesh(const std::string& name, const std::string& path)
		: m_Name(name), m_Path(path)
	{
		ObjectLoader::LoadMesh(name, path, m_Positions, m_Normals, m_TextureCoordinates, m_Submeshes);

		m_GlMesh.reset(CreateGlMesh(true, !m_TextureCoordinates.empty(), !m_Normals.empty()));

		m_PxMesh.reset(CreatePsMesh());
	}

	OpenGL::GlMesh* Mesh::CreateGlMesh(bool positions, bool texcoords, bool normals)
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

					if (positions)
					{
						const glm::vec3& p = m_Positions[face.positionIndex[v]];
						vertices.push_back(p.x);
						vertices.push_back(p.y);
						vertices.push_back(p.z);
					}
					else
					{
						vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(0.0f);
					}

					if (texcoords)
					{
						const glm::vec2& t = m_TextureCoordinates[face.textureCoordinateIndex[v]];
						vertices.push_back(t.x);
						vertices.push_back(t.y);
					}
					else
					{
						vertices.push_back(0.0f); vertices.push_back(0.0f);
					}

					if (normals)
					{
						const glm::vec3& n = m_Normals[face.normalIndex[v]];
						vertices.push_back(n.x);
						vertices.push_back(n.y);
						vertices.push_back(n.z);
					}
					else
					{
						vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(0.0f);
					}

					indices.push_back(indexCount++);
				}
			}
			submeshIndices.insert(indices);
			indices.clear();
		}

		OpenGL::GlVertexBufferLayout layout = {
				{ OpenGL::GlShaderDataType::Float3, "a_Position" },
				{ OpenGL::GlShaderDataType::Float2, "a_TexCoord" },
				{ OpenGL::GlShaderDataType::Float3, "a_Normals" }
			};

		OpenGL::GlMesh* mesh = new OpenGL::GlMesh(m_Name, vertices, layout);

		for (auto sm : submeshIndices)
			mesh->AddSubmesh(sm);

		return mesh;
	}

	Physics::PsMesh* Mesh::CreatePsMesh()
	{
		std::vector<uint32_t> indices;
		for (auto& submeshe : m_Submeshes)
			for (auto& face : submeshe.Faces)
				for (uint16_t v = 0; v < face.vertices; v++)
					indices.push_back(face.positionIndex[v]);

		return new Physics::PsMesh(m_Positions, indices);
	}

	void Mesh::Reload()
	{
		m_Positions.clear();
		m_Normals.clear();
		m_TextureCoordinates.clear();
		m_Submeshes.clear();

		ObjectLoader::LoadMesh(m_Name, m_Path, m_Positions, m_Normals, m_TextureCoordinates, m_Submeshes);

		m_GlMesh.reset(CreateGlMesh(true, !m_TextureCoordinates.empty(), !m_Normals.empty()));

		m_PxMesh.reset(CreatePsMesh());
	}

}