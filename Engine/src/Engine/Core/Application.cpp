#include "egpch.h"
#include "Application.h"
#include "Base.h"

#include "Platform/Platform.h"
#include "Platform/OpenGL/OpenGLAPI.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Audio/SoundEngine.h"
#include "Engine/Physics/PhysicsAPI.h"

namespace Engine {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		Log::Init();

		s_Instance = this;

		m_Window = Scope<Window>(Window::Create());
		m_Window->SetEventCallback(EG_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();
		Renderer2D::Init();
		SoundEngine::Init();
		PhysicsAPI::Init();

		m_LayerStack = new LayerStack();
	}

	Application::~Application() {
		delete m_LayerStack;

		Renderer::Shutdown();
		Renderer2D::Shutdown();
		SoundEngine::Shutdown();
		PhysicsAPI::Shutdown();
	}

	void Application::OnEvent(Event& e)
	{
		EventHandler handler(e);
		handler.Handle<WindowCloseEvent>(EG_BIND_EVENT_FN(Application::OnWindowClose));
		handler.Handle<WindowResizeEvent>(EG_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack->rbegin(); it != m_LayerStack->rend(); it++)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack->PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack->PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Remove(Layer* layer) {
		m_RemovedLayers.push_back(layer);
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = Platform::GetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (Layer* layer : m_RemovedLayers) {
				m_LayerStack->Pop(layer);
			}
			m_RemovedLayers.clear();

			if (!m_Minimized)
			{
				OpenGL::API::Clear();

				for (Layer* layer : *m_LayerStack)
					layer->OnUpdate(timestep);
			}

			m_Window->OnUpdate();
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;
			return true;
		}
		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

}