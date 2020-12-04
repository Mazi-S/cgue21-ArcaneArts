#include "egpch.h"
#include "Scene.h"

#include "Engine/Core/Application.h"

#include "Components.h"
#include "Systems.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Scene/Entity.h"

#include <glm/glm.hpp>



namespace Engine {

	static void UnbindScript(entt::registry& registry, entt::entity entity)
	{
		registry.get<NativeScriptComponent>(entity).Unbind();
	}

	Scene::Scene()
	{
		m_Registry.on_destroy<NativeScriptComponent>().connect<&UnbindScript>();

		// todo
		//m_SpectatorCamera->SetViewportSize(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
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
		System::CharacterController::OnUpdate(m_Registry, ts);

		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) { nsc.Instance->OnUpdate(ts); });


		// todo: physics..

	}

	void Scene::OnRender()
	{
		// Render materials
		Camera camera = System::Camera::GetCamera(m_Registry);

		Renderer::BeginScene(camera);
		{
			auto group = m_Registry.group<TransformComponent>(entt::get<MaterialComponent, MeshComponent>);
			for (auto entity : group)
			{
				auto [transform, material, mesh] = group.get<TransformComponent, MaterialComponent, MeshComponent>(entity);
				Renderer::Submit(mesh, material, transform);
			}
		}

		Renderer::EndScene();
	}

	void Scene::OnEvent(Event& event)
	{
		EventHandler eventHandler(event);
		eventHandler.Handle<WindowResizeEvent>(EG_BIND_EVENT_FN(Scene::OnWindowResize));
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

		// Resize camera components
		System::Camera::SetViewportSize(m_Registry, width, height);
	}

	std::pair<uint32_t, uint32_t> Scene::GetVieportSize()
	{
		return std::pair<uint32_t, uint32_t>(m_ViewportWidth, m_ViewportHeight);
	}

}