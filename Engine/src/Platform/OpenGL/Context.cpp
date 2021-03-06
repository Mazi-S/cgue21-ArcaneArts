#include "egpch.h"
#include "Context.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Engine::OpenGL {

	Context::Context(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		ASSERT(windowHandle, "Window handle is null!");
	}

	void Context::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ASSERT(status, "Failed to initialize Glad!");

		LOG_INFO("OpenGL Info:\n  Vendor: {}\n  Renderer: {}\n  Version: {}", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));
	}

	void Context::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}