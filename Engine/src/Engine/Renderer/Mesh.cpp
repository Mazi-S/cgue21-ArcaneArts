#include "egpch.h"
#include "Mesh.h"
#include "ObjectLoader.h"

namespace Engine {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Mesh ///////////////////////////////////////////////////////////////////////////////////////

	Mesh::Mesh(const std::string& name, Ref<VertexBuffer> vertexBuffer)
		: m_Name(name), m_VertexBuffer(vertexBuffer)
	{ }

	void Mesh::AddSubmesh(std::vector<uint32_t>& indices)
	{
		Ref<IndexBuffer> ib = IndexBuffer::Create(indices.data(), indices.size());
		AddSubmesh(ib);
	}

	void Mesh::AddSubmesh(const Ref<IndexBuffer>& indexBuffer)
	{
		auto subMesh = CreateRef<Submesh>(m_VertexBuffer, indexBuffer);
		m_Submeshes.push_back(subMesh);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Submesh ////////////////////////////////////////////////////////////////////////////////////

	Submesh::Submesh(const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer)
	{
		m_VertexArray = VertexArray::Create();
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);
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