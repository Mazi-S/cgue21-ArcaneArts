#include "egpch.h"
#include "Mesh.h"

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
}