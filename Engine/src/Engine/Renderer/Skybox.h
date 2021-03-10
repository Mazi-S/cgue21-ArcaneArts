#pragma once
#include "Camera.h"
#include "Engine/Core/Base.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Engine {

	class Skybox
	{
	public:
		Skybox();
		~Skybox() = default;

		void Draw(Camera camera);

	private:
		Ref<OpenGL::GlShader> m_SkyboxShader;
		Ref<OpenGL::GlShader> m_CubemapShader;
	};
}
