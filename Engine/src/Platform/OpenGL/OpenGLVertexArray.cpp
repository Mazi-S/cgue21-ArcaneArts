#include "egpch.h"
#include "OpenGLUtil.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Engine::OpenGL {

	GlVertexArray::GlVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	GlVertexArray::~GlVertexArray()
	{
		m_VertexBuffers.clear();
		m_IndexBuffer = nullptr;
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void GlVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void GlVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void GlVertexArray::AddVertexBuffer(const Ref<GlVertexBuffer>& vertexBuffer)
	{
		ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.GetComponentCount(),
				Util::ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset
			);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
		glBindVertexArray(0);
	}

	void GlVertexArray::SetIndexBuffer(const Ref<GlIndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;

		glBindVertexArray(0);
	}

}