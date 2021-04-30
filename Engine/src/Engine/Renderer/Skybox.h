#pragma once
#include "Camera.h"
#include "Engine/Core/Base.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLCubeTexture.h"

namespace Engine {

	class Skybox
	{
	public:
		Skybox();
		~Skybox() = default;

		void Draw(Camera camera);

	private:
		Ref<OpenGL::GlCubeTexture> m_SkyboxTex;

		Ref<OpenGL::GlShader> m_SkyboxShader;
		Ref<OpenGL::GlShader> m_CubemapShader;
	};
}
