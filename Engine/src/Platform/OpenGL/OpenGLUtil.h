#pragma once
#include "OpenGLBufferLayout.h"

namespace Engine::OpenGL::Util {

	// Returns the size of the given ShaderDataType in bytes
	uint32_t ShaderDataTypeSize(GlShaderDataType type);

	uint32_t ShaderDataTypeToOpenGLBaseType(GlShaderDataType type);

}
