#pragma once

struct GLFWwindow;

namespace Engine::OpenGL {

	class GlContext
	{
	public:
		GlContext(GLFWwindow* windowHandle);

		virtual void Init();
		virtual void SwapBuffers();

	private:
		GLFWwindow* m_WindowHandle;
	};

}

