#include "egpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

static GLenum ShaderDataTypeToOpenGLBaseType(Engine::ShaderDataType type)
{
	switch (type)
	{
	case Engine::ShaderDataType::Float:		return GL_FLOAT;
	case Engine::ShaderDataType::Float2:	return GL_FLOAT;
	case Engine::ShaderDataType::Float3:	return GL_FLOAT;
	case Engine::ShaderDataType::Float4:	return GL_FLOAT;
	case Engine::ShaderDataType::Mat3:		return GL_FLOAT;
	case Engine::ShaderDataType::Mat4:		return GL_FLOAT;
	case Engine::ShaderDataType::Int:		return GL_INT;
	case Engine::ShaderDataType::Int2:		return GL_INT;
	case Engine::ShaderDataType::Int3:		return GL_INT;
	case Engine::ShaderDataType::Int4:		return GL_INT;
	case Engine::ShaderDataType::Bool:		return GL_BOOL;
	}

	ASSERT(false, "Unknown ShaderDataType!");
	return 0;
}

namespace Engine::OpenGL {

	VertexArray::VertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		ASSERT(
			vertexBuffer->GetLayout().GetElements().size(),
			"Vertex Buffer has no layout!"
		);

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
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset
			);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
		glBindVertexArray(0);
	}

	void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;

		glBindVertexArray(0);
	}

}