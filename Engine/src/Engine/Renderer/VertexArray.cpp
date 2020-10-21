#include "egpch.h"
#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Engine {

	Ref<VertexArray> VertexArray::Create()
	{
		return CreateRef<OpenGL::VertexArray>();
	}

}