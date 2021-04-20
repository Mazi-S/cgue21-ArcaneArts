#pragma once
#include "Base.h"
#include "Engine/Events/Event.h"

namespace Engine {

	struct WindowSpecification
	{
		std::string Title = "Engine";
		int Width = 1280;
		int Height = 750;
		int RefreshRate = -1;
		bool fullscreen = false;
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
	};

}