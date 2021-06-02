#include "egpch.h"
#include "Scene.h"

#include "Engine/Core/Application.h"
#include "Engine/Util/Math.h"

#include "Components.h"
#include "Systems.h"
#include "Factories.h"
#include "Entity.h"

// Renderer
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/ShadowMap.h"
#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/Light.h"

#include "Engine/Renderer/MeshLibrary.h"

// Audio
#include "Engine/Audio/SoundEngine.h"

namespace Engine {

	Scene::Scene()
	{
		m_ViewportWidth = Application::Get().GetWindow().GetWidth();
		m_ViewportHeight = Application::Get().GetWindow().GetHeight();
		m_Spectator.SetViewportSize(m_ViewportWidth, m_ViewportHeight);

		m_PhysicsScene = new Physics::PsScene();
		m_PhysicsScene->SetEventCallback(std::bind(&Application::OnEvent, &Application::Get(), std::placeholders::_1));

		InitRegistry();
	}

	Scene::~Scene()
	{
		m_Registry.clear();
		delete m_PhysicsScene;
	}

	Entity Scene::CreateEntity(const std::string& name, uint32_t id)
	{
		entt::entity entity = Factory::CreateEntity(m_Registry, name, id);
		return { entity, &m_Registry };
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::SetMainCamera(Entity entity)
	{
		m_MainCamera = entity.m_EntityHandle;
	}

	void Scene::OnPause()
	{
		m_Registry.view<Component::Physics::CharacterControllerComponent>().each([=](auto entity, auto& component) { component.Active = false; });
	}

	void Scene::OnResume()
	{
		m_Registry.view<Component::Physics::CharacterControllerComponent>().each([=](auto entity, auto& component) { component.Active = true; });
	}

	void Scene::OnUpdate(Timestep ts)
	{
		// Update
		if (m_SpectatorActive)
			m_Spectator.OnUpdate(ts);

		System::Physics::OnUpdateKinematic(m_Registry, ts);
		System::CharacterController::OnUpdate(m_Registry, ts);
		System::Audio::OnUpdate(m_Registry);

		System::Renderer::OnUpdateParticleSystem(m_Registry, ts);

		m_Registry.view<Component::Core::NativeScriptComponent>().each([=](auto entity, auto& nsc) { if (nsc.Active) nsc.Instance->OnUpdate(ts); });
		
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
		Camera camera = GetCamera();

		DirectionalLight dLight = System::Light::GetDirectionalLight(m_Registry);
		std::vector<PointLight> pLights = System::Light::GetPointLights(m_Registry);

		Renderer::BeginScene(camera, dLight, pLights);
		ShadowMap::BeginScene(camera, dLight);

		System::Renderer::Submit(m_Registry);

		ShadowMap::EndScene();
		Renderer::EndScene();

		System::Renderer::OnRenderParticleSystem(m_Registry, camera.ViewProjection(), camera.Position());
	}

	void Scene::OnEvent(Event& event)
	{
		if(m_SpectatorActive)
			m_Spectator.OnEvent(event);

		EventHandler eventHandler(event);
		eventHandler.Handle<WindowResizeEvent>(EG_BIND_EVENT_FN(Scene::OnWindowResize));
		eventHandler.Handle<KeyPressedEvent>(EG_BIND_EVENT_FN(Scene::OnKeyPressed));
		eventHandler.Handle<CollisionEvent>(EG_BIND_EVENT_FN(Scene::OnCollision));

		m_Registry.view<Component::Core::NativeScriptComponent>().each([&](auto entity, auto& nsc) { if (nsc.Active) nsc.Instance->OnEvent(event); });
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

	bool Scene::OnCollision(CollisionEvent& e)
	{
		auto& [first, second] = e.Get();
		first.EmplaceOrReplace<Component::Event::CollisionEventComponent>(second);
		second.EmplaceOrReplace<Component::Event::CollisionEventComponent>(first);
		return true;
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize camera components
		System::Camera::SetViewportSize(m_Registry, width, height);
		m_Spectator.SetViewportSize(width, height);
	}

	std::pair<uint32_t, uint32_t> Scene::GetVieportSize()
	{
		return std::pair<uint32_t, uint32_t>(m_ViewportWidth, m_ViewportHeight);
	}

	Camera Scene::GetCamera()
	{
		if (m_SpectatorActive)
			return m_Spectator.GetCamera();

		return m_MainCamera != entt::null ? System::Camera::GetCamera(m_Registry, m_MainCamera) : System::Camera::GetCamera(m_Registry);
	}

}