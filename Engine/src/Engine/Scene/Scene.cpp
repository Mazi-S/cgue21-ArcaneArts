#include "egpch.h"
#include "Scene.h"

#include "Engine/Core/Application.h"

#include "Components.h"
#include "Systems.h"
#include "Factories.h"

#include "Entity.h"

// Renderer
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/Light.h"

namespace Engine {

	static void UnbindScript(entt::registry& registry, entt::entity entity)
	{
		registry.get<NativeScriptComponent>(entity).Unbind();
	}

	void Scene::TriggerHit(entt::entity triggerEntity, entt::entity otherEntity)
	{
		m_Registry.emplace_or_replace<HitComponent>(triggerEntity, otherEntity);
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

	void Scene::AddRigidComponent(entt::registry& registry, entt::entity entity)
	{
		auto& rc = registry.get<RigidComponent>(entity);
		m_PhysicsScene->AddActor(rc.Actor, entity);
	}

	void Scene::RemoveRigidComponent(entt::registry& registry, entt::entity entity)
	{
		auto& rc = registry.get<RigidComponent>(entity);
		m_PhysicsScene->RemoveActor(rc.Actor);
	}

	void Scene::AddRigidDynamicComponent(entt::registry& registry, entt::entity entity)
	{
		auto& rdc = registry.get<RigidDynamicComponent>(entity);
		m_PhysicsScene->AddActor(rdc.Actor, entity);
	}

	void Scene::RemoveRigidDynamicComponent(entt::registry& registry, entt::entity entity)
	{
		auto& rdc = registry.get<RigidDynamicComponent>(entity);
		m_PhysicsScene->RemoveActor(rdc.Actor);
	}

	void Scene::AddTriggerComponent(entt::registry& registry, entt::entity entity)
	{
		auto* sc = registry.try_get<ShapeComponent>(entity);
		
		ASSERT(sc != nullptr, "Entity has no ShapeComponent!");

		PhysicsAPI::SetSimulation(sc->Shape, false);
		PhysicsAPI::SetTrigger(sc->Shape, true);
	}

	void Scene::RemoveTriggerComponent(entt::registry& registry, entt::entity entity)
	{
		auto* sc = registry.try_get<ShapeComponent>(entity);
		if (sc != nullptr)
		{
			PhysicsAPI::SetTrigger(sc->Shape, false);
			PhysicsAPI::SetSimulation(sc->Shape, true);
		}

	}

	void Scene::AddKinematicComponent(entt::registry& registry, entt::entity entity)
	{
		// RigidDynamicComponent
		auto* rdc = registry.try_get<RigidDynamicComponent>(entity);
		if (rdc != nullptr)
			PhysicsAPI::SetKinematic(rdc->Actor, true);
	}

	void Scene::RemoveKinematicComponent(entt::registry& registry, entt::entity entity)
	{
		// RigidDynamicComponent
		auto* rdc = registry.try_get<RigidDynamicComponent>(entity);
		if (rdc != nullptr)
			PhysicsAPI::SetKinematic(rdc->Actor, false);
	}

	void Scene::AddShapeComponent(entt::registry& registry, entt::entity entity)
	{
		auto sc = registry.get<ShapeComponent>(entity);

		auto* rc = registry.try_get<RigidComponent>(entity);
		if (rc != nullptr)
			rc->Actor->attachShape(*sc.Shape);
		
		auto* rdc = registry.try_get<RigidDynamicComponent>(entity);
		if (rdc != nullptr)
			rdc->Actor->attachShape(*sc.Shape);
	}

	void Scene::RemoveShapeComponent(entt::registry& registry, entt::entity entity)
	{
		auto sc = registry.get<ShapeComponent>(entity);

		auto* rc = registry.try_get<RigidComponent>(entity);
		if (rc != nullptr)
			rc->Actor->detachShape(*sc.Shape);

		auto* rdc = registry.try_get<RigidDynamicComponent>(entity);
		if (rdc != nullptr)
			rdc->Actor->detachShape(*sc.Shape);

		sc.Shape->release();
	}

	Scene::Scene()
	{
		m_ViewportWidth = Application::Get().GetWindow().GetWidth();
		m_ViewportHeight = Application::Get().GetWindow().GetHeight();

		m_Registry.on_destroy<NativeScriptComponent>().connect<&UnbindScript>();
		m_Registry.on_construct<CharacterControllerComponent>().connect<&Scene::AddCharacterController>(*this);
		m_Registry.on_construct<CameraComponent>().connect<&Scene::InitCameraComponent>(*this);

		m_Registry.on_construct<RigidComponent>().connect<&Scene::AddRigidComponent>(*this);
		m_Registry.on_destroy<RigidComponent>().connect<&Scene::RemoveRigidComponent>(*this);

		m_Registry.on_construct<RigidDynamicComponent>().connect<&Scene::AddRigidDynamicComponent>(*this);
		m_Registry.on_destroy<RigidDynamicComponent>().connect<&Scene::RemoveRigidDynamicComponent>(*this);

		m_Registry.on_construct<TriggerComponent>().connect<&Scene::AddTriggerComponent>(*this);
		m_Registry.on_destroy<TriggerComponent>().connect<&Scene::RemoveTriggerComponent>(*this);

		m_Registry.on_construct<KinematicComponent>().connect<&Scene::AddKinematicComponent>(*this);
		m_Registry.on_destroy<KinematicComponent>().connect<&Scene::RemoveKinematicComponent>(*this);

		m_Registry.on_construct<ShapeComponent>().connect<&Scene::AddShapeComponent>(*this);
		m_Registry.on_destroy<ShapeComponent>().connect<&Scene::RemoveShapeComponent>(*this);

		m_PhysicsScene = new Physics::PsScene();
		m_PhysicsScene->SetTriggerCallback(std::bind(&Scene::TriggerHit, this, std::placeholders::_1, std::placeholders::_2));
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
		System::Physics::OnUpdateKinematic(m_Registry, ts);
		System::CharacterController::OnUpdate(m_Registry, ts);

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