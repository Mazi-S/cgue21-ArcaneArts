#pragma once
#include "Engine/Core/Base.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Engine {

	class ShaderPanel {
	public:
		ShaderPanel() = default;
		~ShaderPanel() = default;

		void OnImGui();

	private:
		bool m_Active = true;

		Ref<OpenGL::GlShader> m_SelectionContext;

	private:
		void DrawShaderNode(Ref<OpenGL::GlShader>& shader);
		void DrawShader(Ref<OpenGL::GlShader>& shader);
	};

}