#pragma once
#include "Engine/Core/Window.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include <GLFW/glfw3.h>

namespace Engine {

	class WindowImpl : public Window
	{
	public:
		WindowImpl(const WindowSpecification& props);
		virtual ~WindowImpl();

		virtual void OnUpdate() override;

		virtual void SetCursorPosition(float x, float y) override;
		virtual void HideCursor() override;
		virtual void ShowCursor() override;
		virtual void ToggleFullscreen() override;

		// Window attributes
		virtual uint32_t GetWidth() const override { return (uint32_t)m_Data.Width; }
		virtual uint32_t GetHeight() const override { return (uint32_t)m_Data.Height; }
		virtual void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

		virtual GLFWwindow* GetNativeWindow() const { return m_Window; }

	private:
		void Init(const WindowSpecification& props);
		void Shutdown();

	private:

		GLFWwindow* m_Window;
		GLFWmonitor* m_Monitor;
		OpenGL::GlContext* m_Context;

		std::array<int, 2> m_WindowedPos;
		std::array<int, 2> m_WindowedSize;

		struct WindowData
		{
			std::string Title;
			int Width, Height;
			int RefreshRate;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}
