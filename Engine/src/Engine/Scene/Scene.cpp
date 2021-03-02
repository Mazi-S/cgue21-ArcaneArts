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

	void Scene::AddCharacterController(entt::registry& registry, entt::entity entity)
	{
		auto& [tc, ccc] = registry.get<TransformComponent, CharacterControllerComponent>(entity);
		ccc.Controller = m_PhysicsScene->CreateController(ccc.StandingHeight - 2.0f * ccc.Radius, ccc.Radius, tc.Translation);
	}

	void Scene::AddRigidDynamic(entt::registry& registry, entt::entity entity)
	{
		auto& rdc = registry.get<RigidDynamicComponent>(entity);
		m_PhysicsScene->AddActor(rdc.Actor);

		auto* kc = registry.try_get<KinematicMovementComponent>(entity);
		if (kc != nullptr)
			PhysicsAPI::SetKinematic(rdc.Actor, true);
	}

	void Scene::RemoveRigidDynamic(entt::registry& registry, entt::entity entity)
	{
		auto& rsc = registry.get<RigidDynamicComponent>(entity);
		m_PhysicsScene->RemoveActor(rsc.Actor);
	}

	void Scene::AddRigidStatic(entt::registry& registry, entt::entity entity)
	{
		auto& rsc = registry.get<RigidStaticComponent>(entity);
		m_PhysicsScene->AddActor(rsc.Actor);
	}

	void Scene::RemoveRigidStatic(entt::registry& registry, entt::entity entity)
	{
		auto& rsc = registry.get<RigidStaticComponent>(entity);
		m_PhysicsScene->RemoveActor(rsc.Actor);
	}

	void Scene::AddRigidKinemetic(entt::registry& registry, entt::entity entity)
	{
		auto& rkc = registry.get<RigidKinematicComponent>(entity);
		m_PhysicsScene->AddActor(rkc.Actor);
	}

	void Scene::RemoveRigidKinemetic(entt::registry& registry, entt::entity entity)
	{
		auto& rkc = registry.get<RigidKinematicComponent>(entity);
		m_PhysicsScene->RemoveActor(rkc.Actor);
	}

	void Scene::AddKinematicMovementComponent(entt::registry& registry, entt::entity entity)
	{
		auto* rdc = registry.try_get<RigidDynamicComponent>(entity);
		if (rdc != nullptr)
			PhysicsAPI::SetKinematic(rdc->Actor, true);
	}

	void Scene::RemoveKinematicMovementComponent(entt::registry& registry, entt::entity entity)
	{
		auto* rdc = registry.try_get<RigidDynamicComponent>(entity);
		if (rdc != nullptr)
			PhysicsAPI::SetKinematic(rdc->Actor, false);
	}

	Scene::Scene()
	{
		m_ViewportWidth = Application::Get().GetWindow().GetWidth();
		m_ViewportHeight = Application::Get().GetWindow().GetHeight();

		m_Registry.on_destroy<NativeScriptComponent>().connect<&UnbindScript>();
		m_Registry.on_construct<CameraComponent>().connect<&Scene::InitCameraComponent>(*this);
		m_Registry.on_construct<RigidDynamicComponent>().connect<&Scene::AddRigidDynamic>(*this);
		m_Registry.on_construct<RigidStaticComponent>().connect<&Scene::AddRigidStatic>(*this);
		m_Registry.on_construct<RigidKinematicComponent>().connect<&Scene::AddRigidKinemetic>(*this);
		m_Registry.on_construct<CharacterControllerComponent>().connect<&Scene::AddCharacterController>(*this);

		m_Registry.on_construct<KinematicMovementComponent>().connect<&Scene::AddKinematicMovementComponent>(*this);
		m_Registry.on_destroy<KinematicMovementComponent>().connect<&Scene::RemoveKinematicMovementComponent>(*this);
		m_Registry.on_destroy<RigidDynamicComponent>().connect<&Scene::RemoveRigidDynamic>(*this);
		m_Registry.on_destroy<RigidStaticComponent>().connect<&Scene::RemoveRigidStatic>(*this);
		m_Registry.on_destroy<RigidKinematicComponent>().connect<&Scene::RemoveRigidKinemetic>(*this);
	
		m_PhysicsScene = new Physics::PsScene();
	}

	Scene::~Scene()
	{
		delete m_PhysicsScene;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		entt::entity entity = Factory::CreateEntity(m_Registry, name);
		return { entity, this };
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::SetMainCamera(Entity entity)
	{
		m_MainCamera = entity.m_EntityHandle;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		// Update
		System::CharacterController::OnUpdate(m_Registry, ts);
		System::Physics::OnUpdateKinematic(m_Registry, ts);

		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) { if (nsc.Active) nsc.Instance->OnUpdate(ts); });

		
		static float t = 0;
		t += ts;
		if (t > (1.0f / 60.0f))
		{
			t -= (1.0f / 60.0f);
			m_PhysicsScene->Simulate(1.0f / 60.0f);

			System::Physics::OnUpdate(m_Registry);
		}
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