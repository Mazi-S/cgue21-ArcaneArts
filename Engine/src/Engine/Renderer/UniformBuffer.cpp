#include "egpch.h"
#include "UniformBuffer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Engine {

	UniformBufferLayout::UniformBufferLayout(const std::initializer_list<BufferElement>& elements)
	{
		uint32_t offset = 0;
		for (auto& element : elements)
		{
			m_Elements[element.Name] = element;
			m_Elements[element.Name].Offset = offset;
			offset += element.Size;
			m_Size += element.Size;
		}
	}

	Ref<UniformBuffer> UniformBuffer::Create(UniformBufferLayout layout)
	{
		return CreateRef<OpenGL::UniformBuffer>(layout);
	}

}