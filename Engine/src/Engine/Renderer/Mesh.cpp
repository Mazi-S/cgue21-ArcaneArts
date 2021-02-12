#include "egpch.h"
#include "Mesh.h"
#include "Engine/Util/ObjectLoader.h"

#include <set>

namespace Engine {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Mesh ///////////////////////////////////////////////////////////////////////////////////////

	Mesh::Mesh(const std::string& name, std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& textureCoordinates, std::vector<Submesh>& submeshes)
		: m_Name(name), m_Positions(positions), m_Normals(normals), m_TextureCoordinates(textureCoordinates), m_Submeshes(submeshes)
	{
		m_GlMesh = CreateGlMesh(true, true, true, {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float3, "a_Normals" }
			});

		m_PxMesh = CreatePxMesh();
	}

	Ref<OpenGL::GlMesh> Mesh::CreateGlMesh(bool positions, bool texcoords, bool normals, VertexBufferLayout layout)
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

					vertices.push_back(p.x);
					vertices.push_back(p.y);
					vertices.push_back(p.z);
					vertices.push_back(t.x);
					vertices.push_back(t.y);
					vertices.push_back(n.x);
					vertices.push_back(n.y);
					vertices.push_back(n.z);

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

	Ref<Physics::PxMesh> Mesh::CreatePxMesh()
	{
		std::vector<uint32_t> indices;
		for (auto& submeshe : m_Submeshes)
			for (auto& face : submeshe.Faces)
				for (uint16_t v = 0; v < face.vertices; v++)
					indices.push_back(face.positionIndex[v]);

		return CreateRef<Physics::PxMesh>(m_Positions, indices);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	// MeshLibrary ////////////////////////////////////////////////////////////////////////////////

	std::unordered_map<std::string, Ref<Mesh>> MeshLibrary::s_Meshes;

	void MeshLibrary::Add(const Ref<Mesh>& mesh)
	{
		auto& name = mesh->GetName();
		ASSERT(!Exists(name), "Mesh already exists!");
		s_Meshes[name] = mesh;
	}

	Ref<Mesh> MeshLibrary::Load(const std::string& name, const std::string& filepath)
	{
		Ref<Mesh> mesh = ObjectLoader::LoadMesh(name, filepath);
		Add(mesh);
		return mesh;
	}

	Ref<Mesh> MeshLibrary::Get(const std::string& name)
	{
		ASSERT(Exists(name), "Mesh not found!");
		return s_Meshes[name];
	}

	bool MeshLibrary::Exists(const std::string& name)
	{
		return s_Meshes.find(name) != s_Meshes.end();
	}

}