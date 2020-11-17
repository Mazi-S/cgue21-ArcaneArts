#include "egpch.h"
#include "UniformBuffer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Engine {

	static uint32_t Offset_std140(ShaderDataType type)
	{
		switch (type)
		{
		case Engine::ShaderDataType::None:		return 0;
		case Engine::ShaderDataType::Float:		return 4;
		case Engine::ShaderDataType::Float2:	return 4 * 2;
		case Engine::ShaderDataType::Float3:
		case Engine::ShaderDataType::Float4:	return 4 * 4;
		case Engine::ShaderDataType::Mat3:		return 4 * 4 * 3;
		case Engine::ShaderDataType::Mat4:		return 4 * 4 * 4;
		case Engine::ShaderDataType::Int:		return 4;
		case Engine::ShaderDataType::Int2:		return 4 * 2;
		case Engine::ShaderDataType::Int3:
		case Engine::ShaderDataType::Int4:		return 4 * 4;
		case Engine::ShaderDataType::Bool:		return 1;
		}

		ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	UniformBufferLayout::UniformBufferLayout(const std::initializer_list<BufferElement>& elements)
	{
		uint32_t offset = 0;
		for (auto& element : elements)
		{
			m_Elements[element.Name] = element;
			m_Elements[element.Name].Offset = offset;
			offset += Offset_std140(element.Type);
			m_Size += Offset_std140(element.Type);
		}
	}

	Ref<UniformBuffer> UniformBuffer::Create(UniformBufferLayout layout)
	{
		return CreateRef<OpenGL::UniformBuffer>(layout);
	}

}