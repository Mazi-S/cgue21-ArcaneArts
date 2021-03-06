#include "egpch.h"
#include "OpenGLUtil.h"
#include <glad/glad.h>

namespace Engine::OpenGL::Util {

	uint32_t ShaderDataTypeSize(GlShaderDataType type)
	{
		switch (type)
		{
		case GlShaderDataType::None:		return 0;
		case GlShaderDataType::Float:		return 4;
		case GlShaderDataType::Float2:		return 4 * 2;
		case GlShaderDataType::Float3:		return 4 * 3;
		case GlShaderDataType::Float4:		return 4 * 4;
		case GlShaderDataType::Mat3:		return 4 * 3 * 3;
		case GlShaderDataType::Mat4:		return 4 * 4 * 4;
		case GlShaderDataType::Int:			return 4;
		case GlShaderDataType::Int2:		return 4 * 2;
		case GlShaderDataType::Int3:		return 4 * 3;
		case GlShaderDataType::Int4:		return 4 * 4;
		case GlShaderDataType::Bool:		return 1;
		}

		ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	GLenum ShaderDataTypeToOpenGLBaseType(GlShaderDataType type)
	{
		switch (type)
		{
		case Engine::OpenGL::GlShaderDataType::Float:		return GL_FLOAT;
		case Engine::OpenGL::GlShaderDataType::Float2:		return GL_FLOAT;
		case Engine::OpenGL::GlShaderDataType::Float3:		return GL_FLOAT;
		case Engine::OpenGL::GlShaderDataType::Float4:		return GL_FLOAT;
		case Engine::OpenGL::GlShaderDataType::Mat3:		return GL_FLOAT;
		case Engine::OpenGL::GlShaderDataType::Mat4:		return GL_FLOAT;
		case Engine::OpenGL::GlShaderDataType::Int:			return GL_INT;
		case Engine::OpenGL::GlShaderDataType::Int2:		return GL_INT;
		case Engine::OpenGL::GlShaderDataType::Int3:		return GL_INT;
		case Engine::OpenGL::GlShaderDataType::Int4:		return GL_INT;
		case Engine::OpenGL::GlShaderDataType::Bool:		return GL_BOOL;
		}

		ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

}


