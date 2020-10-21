#include "egpch.h"
#include "Context.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Engine::OpenGL {

	Context::Context(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		//TB_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void Context::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ASSERT(status, "Faild to initialize Glad!");

		//LOG_CORE_INFO("OpenGL Info:");
		//LOG_CORE_INFO("   Vendor: {}", glGetString(GL_VENDOR));
		//LOG_CORE_INFO("   Renderer: {}", glGetString(GL_RENDERER));
		//LOG_CORE_INFO("   Version: {}", glGetString(GL_VERSION));
	}

	void Context::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}