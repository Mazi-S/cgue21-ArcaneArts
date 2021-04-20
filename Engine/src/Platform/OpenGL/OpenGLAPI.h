#pragma once
#include <glm/glm.hpp>
#include <Engine.h>

namespace Engine::OpenGL {

	enum class DepthFunction : uint32_t
	{
		Default = 0x0203,
		LEQUAL = 0x0203,
		LESS = 0x0201
	};

	enum class Face : uint32_t
	{
		Default = 0x0405,
		Front = 0x0404,
		Back = 0x0405
	};

	class API
	{
	public:
		static void Init();

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		static void SetClearColor(const glm::vec4 & color);
		static void Clear();
		static void CullFaces(Face face = Face::Back);
		static void DepthFunc(DepthFunction func = DepthFunction::LEQUAL);

		static void DrawIndexed(const Ref<OpenGL::GlVertexArray>& vertexArray, uint32_t indexCount);
		static void DrawIndexed(const OpenGL::GlVertexArray* vertexArray, uint32_t indexCount);

		static void UnbindVertexArray();
	};

}

