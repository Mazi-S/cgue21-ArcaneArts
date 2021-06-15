#pragma once

#include <entt.hpp>
#include <glm/glm.hpp>

#include "Engine/Core/Timestep.h"
#include "Engine/Core/Application.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/PhysicsEvent.h"
#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/Skybox.h"

#include "Engine/Physics/PhysicsAPI.h"
#include "Engine/Physics/PhysicsScene.h"

#include "Spectator.h"

class ExampleLayer;

namespace Engine {

	class Scene
	{
		friend class ExampleLayer;

		friend class Entity;
		friend class ScriptableEntity;
		
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;

	public:
		Scene();
		~Scene();

		// ECS
		Entity CreateEntity(const std::string& name = std::string(), uint32_t id = 0);
		void DestroyEntity(Entity entity);

		void SetMainCamera(Entity entity);
		void SetSkybox(Ref<Skybox> skybox) { m_Skybox = skybox; }

		void OnStart();
		void OnPause();
		void OnResume();
		void OnStop();

		void OnUpdate(Timestep ts);
		void OnRender();
		void OnEvent(Event& event);

		void OnViewportResize(uint32_t width, uint32_t height);

		std::pair<uint32_t, uint32_t> GetVieportSize();
		Camera Scene::GetCamera();

	private:
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnCollision(CollisionEvent& e);


		// Registry
		void InitRegistry();

		// Registry - Core
		void UpdateTransform(entt::registry& registry, entt::entity entity); // TODO
		
		// Registry - Renderer
		void InitCameraComponent(entt::registry& registry, entt::entity entity);
		void DestroyCameraComponent(entt::registry& registry, entt::entity entity);

		void InitParticleSystem(entt::registry& registry, entt::entity entity);
		void DestroyParticleSystem(entt::registry& registry, entt::entity entity);

		// Registry - PhysX
		void InitPhysicsMaterial(entt::registry& registry, entt::entity entity);
		void UpdatePhysicsMaterial(entt::registry& registry, entt::entity entity);

		void InitStaticCollider(entt::registry& registry, entt::entity entity);
		void UpdateStaticCollider(entt::registry& registry, entt::entity entity);
		void DestroyStaticCollider(entt::registry& registry, entt::entity entity);

		void InitDynamicConvex(entt::registry& registry, entt::entity entity);
		void UpdateDynamicConvex(entt::registry& registry, entt::entity entity);
		void DestroyDynamicConvex(entt::registry& registry, entt::entity entity);

		void InitCharacterController(entt::registry& registry, entt::entity entity);
		void UpdateCharacterController(entt::registry& registry, entt::entity entity);
		void DestroyCharacterController(entt::registry& registry, entt::entity entity);

		void AddRigidComponent(entt::registry& registry, entt::entity entity);
		void RemoveRigidComponent(entt::registry& registry, entt::entity entity);
		
		void AddRigidDynamicComponent(entt::registry& registry, entt::entity entity);
		void RemoveRigidDynamicComponent(entt::registry& registry, entt::entity entity);

		// Registry - Audio
		void InitSound2DComponent(entt::registry& registry, entt::entity entity);
		void UpdateSound2DComponent(entt::registry& registry, entt::entity entity);
		void DestroySound2DComponent(entt::registry& registry, entt::entity entity);

		void InitSound3DComponent(entt::registry& registry, entt::entity entity);
		void UpdateSound3DComponent(entt::registry& registry, entt::entity entity);
		void DestroySound3DComponent(entt::registry& registry, entt::entity entity);

	private:
		entt::registry m_Registry;
		entt::entity m_MainCamera = entt::null;

		Engine::Ref<Engine::Skybox> m_Skybox;
		Physics::PsScene* m_PhysicsScene;

		uint32_t m_ViewportWidth, m_ViewportHeight;

		bool m_SceneFocused = true, m_SceneHovered = true;

		float m_PhysicsTime = 0;

		// Spectator
		bool m_SpectatorActive = false;
		Spectator m_Spectator;
	};

}
