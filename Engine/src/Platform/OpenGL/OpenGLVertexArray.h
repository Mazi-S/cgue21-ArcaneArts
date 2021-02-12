#pragma once
#include "Engine/Renderer/VertexArray.h"

namespace Engine::OpenGL {

	class VertexArray : public Engine::VertexArray {
	public:
		VertexArray();
		virtual ~VertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<Engine::VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<Engine::IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Ref<Engine::VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const Ref<Engine::IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
	private:
		uint32_t m_RendererID;
		std::vector<Ref<Engine::VertexBuffer>> m_VertexBuffers;
		Ref<Engine::IndexBuffer> m_IndexBuffer;
	};


}

