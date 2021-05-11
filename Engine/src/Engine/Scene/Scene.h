#pragma once

#include "entt.hpp"

#include "Engine/Core/Timestep.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/PhysicsEvent.h"
#include "Engine/Core/Application.h"
#include "Engine/Renderer/Camera.h"

#include "Engine/Physics/PhysicsAPI.h"
#include "Engine/Physics/PhysicsScene.h"
#include "Engine/Renderer/Camera.h"

#include <glm/glm.hpp>

namespace Engine {

	class Scene
	{
		friend class Entity;
		friend class ScriptableEntity;
		
		friend class SceneHierarchyPanel;

	public:
		Scene();
		~Scene();

		// ECS
		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void SetMainCamera(Entity entity);

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

		// Registry - PhysX
		void InitStaticCollider(entt::registry& registry, entt::entity entity);
		void UpdateStaticCollider(entt::registry& registry, entt::entity entity);
		void DestroyStaticCollider(entt::registry& registry, entt::entity entity);

		void InitCharacterController(entt::registry& registry, entt::entity entity);
		void UpdateCharacterController(entt::registry& registry, entt::entity entity);
		void DestroyCharacterController(entt::registry& registry, entt::entity entity);

		void AddRigidComponent(entt::registry& registry, entt::entity entity);
		void RemoveRigidComponent(entt::registry& registry, entt::entity entity);
		
		void AddRigidDynamicComponent(entt::registry& registry, entt::entity entity);
		void RemoveRigidDynamicComponent(entt::registry& registry, entt::entity entity);

		// Registry - Audio
		void AddSound2DComponent(entt::registry& registry, entt::entity entity);
		void RemoveSound2DComponent(entt::registry& registry, entt::entity entity);

		void AddSound3DComponent(entt::registry& registry, entt::entity entity);
		void RemoveSound3DComponent(entt::registry& registry, entt::entity entity);
	private:
		entt::registry m_Registry;
		entt::entity m_MainCamera = entt::null;

		Physics::PsScene* m_PhysicsScene;

		uint32_t m_ViewportWidth, m_ViewportHeight;

		bool m_SceneFocused = true, m_SceneHovered = true;
	};

}
