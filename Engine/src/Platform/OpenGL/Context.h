#pragma once

#include "Engine/Renderer/RenderContext.h"

struct GLFWwindow;

namespace Engine::OpenGL {

	class Context : public RenderContext
	{
	public:
		Context(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};

}

