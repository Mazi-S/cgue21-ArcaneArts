#include "egpch.h"
#include "Scene.h"

#include "Engine/Core/Application.h"

#include "Components.h"
#include "Systems.h"
#include "Factories.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/Light.h"
#include "Engine/Scene/Entity.h"

#include <glm/glm.hpp>

namespace Engine {

	static void UnbindScript(entt::registry& registry, entt::entity entity)
	{
		registry.get<NativeScriptComponent>(entity).Unbind();
	}

	void Scene::InitCameraComponent(entt::registry& registry, entt::entity entity)
	{
		System::Camera::SetViewportSize(registry, entity, m_ViewportWidth, m_ViewportHeight);
	}

	Scene::Scene()
	{
		m_ViewportWidth = Application::Get().GetWindow().GetWidth();
		m_ViewportHeight = Application::Get().GetWindow().GetHeight();

		m_Registry.on_destroy<NativeScriptComponent>().connect<&UnbindScript>();
		m_Registry.on_construct<CameraComponent>().connect<&Scene::InitCameraComponent>(*this);
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		entt::entity entity = Factory::CreateEntity(m_Registry, name);
		return { entity, this };
	}

	Entity Scene::CreateMainCamera(Entity parent)
	{
		if (m_MainCamera != entt::null)
			m_Registry.destroy(m_MainCamera);

		m_MainCamera = Factory::CreateCamera(m_Registry, parent.m_EntityHandle);
		return { m_MainCamera, this };
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(Timestep ts)
	{
		// Update
		System::CharacterController::OnUpdate(m_Registry, ts);

		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) { if (nsc.Active) nsc.Instance->OnUpdate(ts); });
		
		System::Physics::OnUpdate(m_Registry, ts);
	}

	void Scene::OnRender()
	{
		// Render materials
		Camera camera = m_MainCamera != entt::null ? System::Camera::GetCamera(m_Registry, m_MainCamera) : System::Camera::GetCamera(m_Registry);
		DirectionalLight dLight = System::Light::GetDirectionalLight(m_Registry);
		std::vector<PointLight> pLights = System::Light::GetPointLights(m_Registry);
		PointLight pLight = pLights.size() >= 1 ? pLights[0] : PointLight();

		Renderer::BeginScene(camera, dLight, pLight);
		System::Renderer::Submit(m_Registry);
		Renderer::EndScene();
	}

	void Scene::OnEvent(Event& event)
	{
		EventHandler eventHandler(event);
		eventHandler.Handle<WindowResizeEvent>(EG_BIND_EVENT_FN(Scene::OnWindowResize));
		eventHandler.Handle<KeyPressedEvent>(EG_BIND_EVENT_FN(Scene::OnKeyPressed));

		m_Registry.view<NativeScriptComponent>().each([&](auto entity, auto& nsc) { if (nsc.Active) nsc.Instance->OnEvent(event); });
	}

	bool Scene::OnWindowResize(WindowResizeEvent& e)
	{
		OnViewportResize(e.GetWidth(), e.GetHeight());
		return false;
	}

	bool Scene::OnKeyPressed(KeyPressedEvent& e)
	{
		return System::CharacterController::OnKeyPressed(m_Registry, e);
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

	Camera Scene::GetCamera()
	{
		Camera camera = m_MainCamera != entt::null ? System::Camera::GetCamera(m_Registry, m_MainCamera) : System::Camera::GetCamera(m_Registry);
		return camera;
	}

}