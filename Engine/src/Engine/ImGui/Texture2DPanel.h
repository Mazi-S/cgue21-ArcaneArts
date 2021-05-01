#pragma once
#include "Engine/Core/Base.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Engine {

	class Texture2DPanel {
	public:
		Texture2DPanel() = default;
		~Texture2DPanel() = default;

		void OnImGui();

	private:
		Ref<OpenGL::GlTexture2D> m_SelectionContext;

	private:
		void DrawTextureNode(Ref<OpenGL::GlTexture2D>& texture);
		void DrawTexture(Ref<OpenGL::GlTexture2D>& texture);
	};

}