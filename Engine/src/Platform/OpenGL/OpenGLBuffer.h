#pragma once
#include "Engine/Renderer/Buffer.h"

namespace Engine::OpenGL {

	class VertexBuffer : public Engine::VertexBuffer
	{
	public:
		VertexBuffer(uint32_t size);
		VertexBuffer(float* vertices, uint32_t size);
		VertexBuffer(const void* vertices, uint32_t size);
		virtual ~VertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;

		virtual const VertexBufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const VertexBufferLayout& layout) override { m_Layout = layout; }

	private:
		uint32_t m_RendererID;
		VertexBufferLayout m_Layout;
	};

	class IndexBuffer : public Engine::IndexBuffer
	{
	public:
		IndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~IndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override { return m_Count; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

}
