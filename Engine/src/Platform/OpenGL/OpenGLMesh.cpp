#include "egpch.h"
#include "OpenGLMesh.h"

namespace Engine::OpenGL {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Mesh ///////////////////////////////////////////////////////////////////////////////////////

	GlMesh::GlMesh(const std::string& name, std::vector<float>& vertices, VertexBufferLayout& layout)
		: m_Name(name), m_Layout(layout)
	{
		m_VertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float));
		m_VertexBuffer->SetLayout(layout);
	}

	void GlMesh::AddSubmesh(std::vector<uint32_t>& indices)
	{
		Ref<Engine::IndexBuffer> ib = IndexBuffer::Create(indices.data(), indices.size());
		auto subMesh = CreateRef<Submesh>(m_VertexBuffer, ib);
		m_Submeshes.push_back(subMesh);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// Submesh ////////////////////////////////////////////////////////////////////////////////////

	Submesh::Submesh(const Ref<Engine::VertexBuffer>& vertexBuffer, const Ref<Engine::IndexBuffer>& indexBuffer)
	{
		m_VertexArray = VertexArray::Create();
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

}
