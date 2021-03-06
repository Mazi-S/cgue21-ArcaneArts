#pragma once

#include <glm/glm.hpp>
#include <Engine.h>

namespace Engine::OpenGL {

	class API
	{
	public:
		static void Init();

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		static void SetClearColor(const glm::vec4 & color);
		static void Clear();

		static void DrawIndexed(const Ref<OpenGL::GlVertexArray>& vertexArray, uint32_t indexCount);
	};

}

