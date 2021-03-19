#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Renderer/Camera.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include "glm/glm.hpp"

namespace Engine {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();
	
		static void BeginScene(const Camera& camera);
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const Ref<OpenGL::GlTexture2D>& texture, const glm::vec4 tintColor = glm::vec4(1));
	};
}