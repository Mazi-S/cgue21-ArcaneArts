#pragma once
#include "Base.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/KeyEvent.h"

#include "Engine/Core/Timestep.h"

#include "Engine/Core/Layer.h"
#include "Engine/Core/LayerStack.h"

#include "Engine/Core/Window.h"

namespace Engine {

	struct ApplicationSpecification
	{
		float Brightness = 1.0;
		float Brightness2D = 1.0;
		float Exposure = 1.0;
	};

	class Application
	{
	public:
		Application(ApplicationSpecification appSpec, WindowSpecification windowSpec);
		virtual ~Application();

		void Run();
		void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer();

		void Remove(Layer* layer);

		void ShowImGui(bool show) { m_ImGui = show; };

		float Brightness() { return m_Brightness; }
		float Brightness2D() { return m_Brightness2D; }

		Window& GetWindow() { return *m_Window; }

		static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnKeyPressed(Engine::KeyPressedEvent& event);

	private:
		Scope<Window> m_Window;

		float m_Brightness = 1.0f;
		float m_Brightness2D = 1.0f;
		bool m_Running = true;
		bool m_Minimized = false;
		bool m_ImGui = false;

		LayerStack* m_LayerStack;
		std::vector<Layer*> m_RemovedLayers;

		Timestep m_LastFrameTime;

		static Application* s_Instance;
	};

}

