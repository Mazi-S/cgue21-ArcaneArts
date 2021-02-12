#pragma once

#include "OpenGLBuffer.h"
#include "OpenGLVertexArray.h"

namespace Engine::OpenGL {

	class Submesh;

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Mesh ///////////////////////////////////////////////////////////////////////////////////////

	class GlMesh
	{
	public:
		GlMesh() = default;
		GlMesh(const std::string& name, std::vector<float>& vertices, VertexBufferLayout& layout);

		void AddSubmesh(std::vector<uint32_t> & indices);

		const std::string GetName() { return m_Name; }
		const std::vector<Ref<Submesh>>& GetSubmeshes() const { return m_Submeshes; }

	private:
		const std::string m_Name;

		Ref<Engine::VertexBuffer> m_VertexBuffer;
		VertexBufferLayout m_Layout;
		std::vector<Ref<Submesh>> m_Submeshes;
	};


	///////////////////////////////////////////////////////////////////////////////////////////////
	// Submesh ////////////////////////////////////////////////////////////////////////////////////

	class Submesh
	{
		friend class GlMesh;

	public:
		Submesh() = default;
		Submesh(const Ref<Engine::VertexBuffer>& vertexBuffer, const Ref<Engine::IndexBuffer>& indexBuffer);

		Ref<Engine::VertexArray> GetVertexArray() { return m_VertexArray; }

	private:
		Ref<Engine::VertexArray> m_VertexArray;
	};
}

