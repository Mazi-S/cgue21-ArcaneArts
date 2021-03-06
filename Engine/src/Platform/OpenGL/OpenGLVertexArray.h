#pragma once
#include "Engine/Core/Base.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Engine::OpenGL {

	class GlVertexArray {
	public:
		GlVertexArray();
		virtual ~GlVertexArray();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual void AddVertexBuffer(const Ref<GlVertexBuffer>& vertexBuffer);
		virtual void SetIndexBuffer(const Ref<GlIndexBuffer>& indexBuffer);

		virtual const std::vector<Ref<GlVertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
		virtual const Ref<GlIndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
	private:
		uint32_t m_RendererID;
		std::vector<Ref<GlVertexBuffer>> m_VertexBuffers;
		Ref<GlIndexBuffer> m_IndexBuffer;
	};

}

