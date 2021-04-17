#pragma once
#include "OpenGLBufferLayout.h"

namespace Engine::OpenGL {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Vertex Buffer //////////////////////////////////////////////////////////////////////////////

	class GlVertexBuffer
	{
	public:
		GlVertexBuffer(uint32_t size);
		GlVertexBuffer(float* vertices, uint32_t size);
		GlVertexBuffer(const void* vertices, uint32_t size);
		virtual ~GlVertexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;
		const uint32_t GetRendererID() const { return m_RendererID; }

		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0);

		virtual const GlVertexBufferLayout& GetLayout() const { return m_Layout; }
		virtual void SetLayout(const GlVertexBufferLayout& layout) { m_Layout = layout; }

	private:
		uint32_t m_RendererID;
		GlVertexBufferLayout m_Layout;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Index Buffer ///////////////////////////////////////////////////////////////////////////////

	class GlIndexBuffer
	{
	public:
		GlIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~GlIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetCount() const { return m_Count; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

}
