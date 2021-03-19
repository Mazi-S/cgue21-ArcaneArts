#pragma once

#include "Engine/Core/Base.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include <glm/glm.hpp>

namespace Engine::Component::Renderer2D {

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;

		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct SpriteRendererTextureComponent
	{
		Ref<OpenGL::GlTexture2D> Texture;

		SpriteRendererTextureComponent() = default;
		SpriteRendererTextureComponent(const SpriteRendererTextureComponent&) = default;

		SpriteRendererTextureComponent(Ref<OpenGL::GlTexture2D> texture)
			: Texture(texture) {}
	};
}