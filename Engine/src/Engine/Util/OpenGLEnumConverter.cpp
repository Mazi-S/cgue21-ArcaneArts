#include "egpch.h"
#include "OpenGLEnumConverter.h"
#include <glad/glad.h>

namespace Engine::Util {

	std::string GlEnumConverter::ToString(uint32_t value)
	{
		switch (value)
		{
		case GL_FALSE:
			return "GL_FALSE";
		case GL_TRUE:
			return "GL_TRUE";
		case GL_DEPTH_COMPONENT:
			return "GL_DEPTH_COMPONENT";
		case GL_RED:
			return "GL_RED";
		case GL_GREEN:
			return "GL_GREEN";
		case GL_BLUE:
			return "GL_BLUE";
		case GL_ALPHA:
			return "GL_ALPHA";
		case GL_RGB:
			return "GL_RGB";
		case GL_RGBA:
			return "GL_RGBA";
		case GL_POINT:
			return "GL_POINT";
		case GL_LINE:
			return "GL_LINE";
		case GL_RGBA16F:
			return "GL_RGBA16F";
		case GL_FILL:
			return "GL_FILL";
		case GL_NEAREST:
			return "GL_NEAREST";
		case GL_LINEAR:
			return "GL_LINEAR";
		case GL_CLAMP_TO_EDGE:
			return "GL_CLAMP_TO_EDGE";
		case GL_MIRRORED_REPEAT:
			return "GL_MIRRORED_REPEAT";
		case GL_NEAREST_MIPMAP_NEAREST:
			return "GL_NEAREST_MIPMAP_NEAREST";
		case GL_LINEAR_MIPMAP_NEAREST:
			return "GL_LINEAR_MIPMAP_NEAREST";
		case GL_NEAREST_MIPMAP_LINEAR:
			return "GL_NEAREST_MIPMAP_LINEAR";
		case GL_LINEAR_MIPMAP_LINEAR:
			return "GL_LINEAR_MIPMAP_LINEAR";
		case GL_REPEAT:
			return "GL_REPEAT";
		case GL_INT:
			return "GL_INT";
		case GL_UNSIGNED_INT:
			return "GL_UNSIGNED_INT";
		case GL_FLOAT:
			return "GL_REPEAT";
		case GL_RGB8:
			return "GL_RGB8";
		case GL_RGBA8:
			return "GL_RGBA8";
		case GL_UNSIGNED_BYTE:
			return "GL_UNSIGNED_BYTE";
		default:
			return "Not specified! (" + std::to_string(value) + ")";
		}

		return std::string();
	}

}