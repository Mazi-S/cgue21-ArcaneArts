#include "egpch.h"
#include "OpenGLMesh.h"

namespace Engine::OpenGL {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Mesh ///////////////////////////////////////////////////////////////////////////////////////

	GlMesh::GlMesh(const std::string& name, std::vector<float>& vertices, GlVertexBufferLayout& layout)
		: m_Name(name), m_Layout(layout)
	{
		m_VertexBuffer = CreateRef<GlVertexBuffer>(vertices.data(), vertices.size() * sizeof(float));
		m_VertexBuffer->SetLayout(layout);
	}

	void GlMesh::AddSubmesh(std::vector<uint32_t>& indices)
	{
		Ref<GlIndexBuffer> ib = CreateRef<GlIndexBuffer>(indices.data(), indices.size());
		auto subMesh = CreateRef<Submesh>(m_VertexBuffer, ib);
		m_Submeshes.push_back(subMesh);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// Sub-mesh ///////////////////////////////////////////////////////////////////////////////////

	Submesh::Submesh(const Ref<GlVertexBuffer>& vertexBuffer, const Ref<GlIndexBuffer>& indexBuffer)
	{
		m_VertexArray = CreateRef<GlVertexArray>();
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

}
