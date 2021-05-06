#include "egpch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>
#include "Platform/OpenGL/OpenGLAPI.h"

namespace Engine::OpenGL {

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// VertexBuffer //////////////////////////////////////////////////////////////////////////////////

	GlVertexBuffer::GlVertexBuffer(uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	GlVertexBuffer::GlVertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	GlVertexBuffer::GlVertexBuffer(const void* vertices, uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	GlVertexBuffer::~GlVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void GlVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void GlVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void GlVertexBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// IndexBuffer ///////////////////////////////////////////////////////////////////////////////////

	GlIndexBuffer::GlIndexBuffer(uint32_t* indices, uint32_t count) : m_Count(count)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	GlIndexBuffer::~GlIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void GlIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void GlIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}
