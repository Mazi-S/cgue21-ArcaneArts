#include "egpch.h"
#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>

namespace Engine::OpenGL {

	UniformBuffer::UniformBuffer(const UniformBufferLayout& layout)
	{
		m_Layout = layout;
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);

		glBufferData(GL_UNIFORM_BUFFER, layout.Size(), NULL, GL_DYNAMIC_DRAW);
	}

	void UniformBuffer::Bind(uint32_t bindingPoint)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_RendererID);
	}

	void UniformBuffer::SetData(const void* data, std::string name)
	{
		const auto& element = m_Layout.GetElement(name);
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
		glBufferSubData(GL_UNIFORM_BUFFER, element.Offset, element.Size, data);
		// todo: use memcpy
	}

}