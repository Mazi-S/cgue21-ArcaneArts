#pragma once
#include "Engine/Core/Window.h"
#include "Engine/Renderer/RenderContext.h"
#include <GLFW/glfw3.h>

namespace Engine {

	class WindowImpl : public Window
	{
	public:
		WindowImpl(const WindowProps& props);
		virtual ~WindowImpl();

		virtual void OnUpdate() override;

		// Window attributes
		virtual uint32_t GetWidth() const override { return m_Data.Width; }
		virtual uint32_t GetHeight() const override { return m_Data.Height; }
		virtual void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

		virtual GLFWwindow* GetNativeWindow() const { return m_Window; }

	private:
		void Init(const WindowProps& props);
		void Shutdown();

	private:
		GLFWwindow* m_Window;
		RenderContext* m_Context;

		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;

	};

}
