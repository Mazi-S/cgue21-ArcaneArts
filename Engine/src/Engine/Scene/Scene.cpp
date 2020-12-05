#include "egpch.h"
#include "Scene.h"

#include "Engine/Core/Application.h"

#include "Components.h"
#include "Systems.h"
#include "Factories.h"

#include "Engine/Renderer/Renderer.h"
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

	Entity Scene::CreateHero()
	{
		if (m_Hero != entt::null)
			DestroyEntity(m_Hero);

		m_Hero = Factory::CreateHero(m_Registry);
		return {m_Hero, this};
	}

	Entity Scene::CreateMainCamera(Entity parent)
	{
		if (m_MainCamera != entt::null)
			DestroyEntity(m_MainCamera);

		m_MainCamera = Factory::CreateCamera(m_Registry, parent.m_EntityHandle);
		return { m_MainCamera, this };
	}

	Entity Scene::CreateMagicBall(bool mainHand)
	{
		entt::entity ball = Factory::CreateMagicBall(m_Registry, m_Hero, mainHand);
		return { ball, this };
	}

	void Scene::DestroyEntity(entt::entity entity)
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
		Camera camera = m_MainCamera != entt::null ? System::Camera::GetCamera(m_Registry, m_MainCamera) : System::Camera::GetCamera(m_Registry);
		Renderer::BeginScene(camera);
		System::Renderer::Submit(m_Registry);
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

	glm::mat4 Scene::GetCharacterTransform()
	{
		auto view = m_Registry.view<HeroComponent, TransformComponent>();
		for (entt::entity e : view)
		{
			return System::GetTransform(m_Registry, e);
		}

		return glm::mat4(1.0f);
	}

}