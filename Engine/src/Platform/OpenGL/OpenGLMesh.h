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
		GlMesh(const std::string& name, std::vector<float>& vertices, GlVertexBufferLayout& layout);

		void AddSubmesh(std::vector<uint32_t> & indices);

		const std::string GetName() { return m_Name; }
		const std::vector<Ref<Submesh>>& GetSubmeshes() const { return m_Submeshes; }

	private:
		const std::string m_Name;

		Ref<GlVertexBuffer> m_VertexBuffer;
		GlVertexBufferLayout m_Layout;
		std::vector<Ref<Submesh>> m_Submeshes;
	};


	///////////////////////////////////////////////////////////////////////////////////////////////
	// Sub-mesh ///////////////////////////////////////////////////////////////////////////////////

	class Submesh
	{
		friend class GlMesh;

	public:
		Submesh() = default;
		Submesh(const Ref<GlVertexBuffer>& vertexBuffer, const Ref<GlIndexBuffer>& indexBuffer);

		Ref<GlVertexArray> GetVertexArray() { return m_VertexArray; }

	private:
		Ref<GlVertexArray> m_VertexArray;
	};
}

