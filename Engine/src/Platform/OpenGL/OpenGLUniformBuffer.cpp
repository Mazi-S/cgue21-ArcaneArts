#include "egpch.h"
#include "OpenGLUniformBuffer.h"
#include "OpenGLUtil.h"

#include <glad/glad.h>

namespace Engine::OpenGL {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// UniformBuffer //////////////////////////////////////////////////////////////////////////////

	GlUniformBuffer::GlUniformBuffer(const GlUniformBufferLayout_std140& layout)
	{
		m_Layout = layout;
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);

		glBufferData(GL_UNIFORM_BUFFER, layout.Size(), NULL, GL_DYNAMIC_DRAW);
	}

	void GlUniformBuffer::Bind(uint32_t bindingPoint)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_RendererID);
	}

	void GlUniformBuffer::SetData(const void* data, std::string name)
	{
		const auto& element = m_Layout.GetElement(name);
		if (element.Type == GlShaderDataType::Mat3)
			SetData(data, 3, Util::ShaderDataTypeSize(GlShaderDataType::Float3), Util::ShaderDataTypeSize(GlShaderDataType::Float4), element.Offset);
		else
			SetData(data, element.Size, element.Offset);
	}

	void GlUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	}

	const GlBufferElement& GlUniformBuffer::GetElement(const std::string& name)
	{
		return m_Layout.GetElement(name);
	}

	void GlUniformBuffer::SetData(const void* data, uint32_t elemCount, uint32_t elemSize, uint32_t elemOffset, uint32_t offset)
	{
		for (uint32_t i = 0; i < elemCount; i++)
			SetData(static_cast<const char*>(data) + elemSize * i, elemSize, offset + elemOffset * i);
	}

}