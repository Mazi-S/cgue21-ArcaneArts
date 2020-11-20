#include "egpch.h"
#include "Scene.h"

#include "Engine/Core/Application.h"

#include "Components.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Scene/Entity.h"

#include <glm/glm.hpp>



namespace Engine {

	Scene::Scene()
	{
		m_SpectatorController = CreateRef<Engine::CameraController>();
		m_SpectatorCamera = CreateRef<Engine::SceneCamera>();
		m_SpectatorCamera->SetViewportSize(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(name.empty() ? "Entity" : name);
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(Timestep ts)
	{
		// Update
		if (m_Spectator)
			m_SpectatorController->OnUpdate(ts);

		// todo: physics..


	}

	void Scene::OnRender()
	{
		// Render materials
		Renderer::BeginScene(m_SpectatorCamera, m_SpectatorController->GetTransform());
		{
			auto group = m_Registry.group<TransformComponent>(entt::get<MaterialComponent, MeshComponent>);
			for (auto entity : group)
			{
				auto [transform, material, mesh] = group.get<TransformComponent, MaterialComponent, MeshComponent>(entity);
				Renderer::Submit(material, mesh, transform);
			}
		}
		Renderer::EndScene();
	}

	void Scene::OnEvent(Event& event)
	{
		Engine::EventHandler eventHandler(event);
		eventHandler.Handle<WindowResizeEvent>(EG_BIND_EVENT_FN(Scene::OnWindowResize));

		if (m_Spectator && m_SceneHovered)
			m_SpectatorController->OnEvent(event);
	}


	bool Scene::OnWindowResize(WindowResizeEvent& e)
	{
		OnViewportResize(e.GetWidth(), e.GetHeight());
		return false;
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize spectator camera
		m_SpectatorCamera->SetViewportSize(width, height);
	}

	std::pair<uint32_t, uint32_t> Scene::GetVieportSize()
	{
		return std::pair<uint32_t, uint32_t>(m_ViewportWidth, m_ViewportHeight);
	}

}