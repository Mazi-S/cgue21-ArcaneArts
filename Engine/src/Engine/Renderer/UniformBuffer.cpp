#include "egpch.h"
#include "UniformBuffer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Engine {

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, const std::initializer_list<BufferElement>& elements)
	{
		OpenGL::BufferLayout_std140 layout(size, elements);
		return CreateRef<OpenGL::UniformBuffer>(layout);
	}

}