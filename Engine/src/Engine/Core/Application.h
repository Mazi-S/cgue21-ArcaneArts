#pragma once
#include "Base.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"

#include "Engine/Core/Timestep.h"

#include "Engine/Core/Layer.h"
#include "Engine/Core/LayerStack.h"

#include "Engine/Core/Window.h"

namespace Engine {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		Window& GetWindow() { return *m_Window; }

		static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		Scope<Window> m_Window;

		bool m_Running = true;
		bool m_Minimized = false;

		LayerStack m_LayerStack;

		Timestep m_LastFrameTime;

		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}

