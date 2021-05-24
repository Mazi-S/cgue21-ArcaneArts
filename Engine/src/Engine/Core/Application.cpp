#include "egpch.h"
#include "Application.h"
#include "Base.h"

#include "Platform/Platform.h"
#include "Platform/OpenGL/OpenGLAPI.h"
#include "Platform/Windows/WindowImpl.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/ShadowMap.h"
#include "Engine/ImGui/ImGuiAPI.h"
#include "Engine/Audio/SoundEngine.h"
#include "Engine/Physics/PhysicsAPI.h"

namespace Engine {

	Application* Application::s_Instance = nullptr;

	Application::Application(ApplicationSpecification appSpec, WindowSpecification windowSpec)
	{
		Log::Init();

		s_Instance = this;

		m_Window = CreateScope<WindowImpl>(windowSpec);
		m_Window->SetEventCallback(EG_BIND_EVENT_FN(Application::OnEvent));

		m_Brightness = appSpec.Brightness;
		m_Brightness2D = appSpec.Brightness2D;

		Renderer::Init();
		Renderer2D::Init();
		SoundEngine::Init();
		PhysicsAPI::Init();
		ImGuiAPI::Init();

		Texture2DLibrary::Init();
		ShaderLibrary::Init();
		MaterialLibrary::Init();
		MeshLibrary::Init();
		SoundLibrary::Init();

		ShadowMap::Init();
		ParticleSystem::Init();

		m_LayerStack = new LayerStack();
	}

	Application::~Application() {
		delete m_LayerStack;

		ImGuiAPI::Shutdown();
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
		handler.Handle<KeyPressedEvent>(EG_BIND_EVENT_FN(Application::OnKeyPressed));

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

			OpenGL::API::Clear();

			if (!m_Minimized)
				for (Layer* layer : *m_LayerStack)
					layer->OnUpdate(timestep);

			// ImGui
			ImGuiAPI::Begin();
			if (m_ImGui)
				for (Layer* layer : *m_LayerStack)
					layer->OnImGui();
			ImGuiAPI::End();

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

	bool Application::OnKeyPressed(Engine::KeyPressedEvent& event)
	{
		if (event.GetKeyCode() == Engine::Key::F4)
		{
			m_ImGui = !m_ImGui;
			return true;
		}
		return false;
	}

}