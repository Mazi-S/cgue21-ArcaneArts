#include "egpch.h"
#include "WindowImpl.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

static bool s_GLFWInitialized = false;

static void GLFWErrorCallback(int error, const char* description)
{
	LOG_ERROR("GLFW Error ({0}): {1}", error, description);
}

namespace Engine {

	WindowImpl::WindowImpl(const WindowSpecification& props)
		: m_Monitor(nullptr), m_WindowedPos({0,0}), m_WindowedSize({props.Width, props.Height})
	{
		Init(props);
	}

	WindowImpl::~WindowImpl()
	{
		Shutdown();
	}

	void WindowImpl::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowImpl::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowImpl::IsVSync() const
	{
		return m_Data.VSync;
	}

	void WindowImpl::Init(const WindowSpecification& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		m_Data.RefreshRate = props.RefreshRate;

		LOG_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);


			s_GLFWInitialized = true;
		}

		if (props.fullscreen)
		{
			m_Monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(m_Monitor);
			m_Data.Width = mode->width;
			m_Data.Height = mode->height;
		}

		m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), m_Monitor, nullptr);

		m_Context = new OpenGL::GlContext(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, false);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, true);
				data.EventCallback(event);
				break;
			}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
	}

	void WindowImpl::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowImpl::SetCursorPosition(float x, float y)
	{
		glfwSetCursorPos(m_Window, x, y);
	}

	void WindowImpl::HideCursor()
	{
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void WindowImpl::ShowCursor()
	{
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void WindowImpl::ToggleFullscreen()
	{
		if (m_Monitor != nullptr)
		{
			glfwSetWindowMonitor(m_Window, nullptr, m_WindowedPos[0], m_WindowedPos[1], m_WindowedSize[0], m_WindowedSize[1], m_Data.RefreshRate);
			m_Monitor = nullptr;
		}
		else
		{
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);

			glfwGetWindowPos(m_Window, &m_WindowedPos[0], &m_WindowedPos[1]);
			glfwGetWindowSize(m_Window, &m_WindowedSize[0], &m_WindowedSize[1]);

			glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height, m_Data.RefreshRate);

			m_Monitor = monitor;
		}
	}


}


