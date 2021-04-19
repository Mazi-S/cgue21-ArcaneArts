#pragma once
#include "Base.h"
#include "Engine/Events/Event.h"

namespace Engine {

	struct WindowProps
	{
		std::string Title;
		int Width;
		int Height;
		int RefreshRate;

		WindowProps(
			const std::string& title = "Engine",
			int width = 1280,
			int height = 750,
			int refreshRate = -1 // -1 = GLFW_DONT_CARE
		) : Title(title), Width(width), Height(height), RefreshRate(refreshRate) { }
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual void SetCursorPosition(float x, float y) = 0;
		virtual void HideCursor() = 0;
		virtual void ShowCursor() = 0;
		virtual void ToggleFullscreen() = 0;

		// Window attributes
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		// Implemented in the platform specific class (Windows)
		static Window* Create(const WindowProps& props = WindowProps());
	};

}