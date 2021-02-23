#pragma once

#include "entt.hpp"

#include "Engine/Core/Timestep.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Core/Application.h"
#include "Engine/Renderer/Camera.h"

#include "Engine/Physics/Physics.h"
#include "Engine/Physics/PhysicsScene.h"
#include "Engine/Renderer/Camera.h"

#include <glm/glm.hpp>

namespace Engine {

	class Scene
	{
		friend class Entity;
		friend class ScriptableEntity;

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

		void InitCameraComponent(entt::registry& registry, entt::entity entity);
		void AddCharacterController(entt::registry& registry, entt::entity entity);
		void AddRegidDynamic(entt::registry& registry, entt::entity entity);
		void AddRegidStatic(entt::registry& registry, entt::entity entity);

		void AddKinematicComponent(entt::registry& registry, entt::entity entity);
		void RemoveKinematicComponent(entt::registry& registry, entt::entity entity);


	private:
		entt::registry m_Registry;
		entt::entity m_MainCamera = entt::null;

		Physics::PsScene* m_PhysicsScene;

		uint32_t m_ViewportWidth, m_ViewportHeight;

		bool m_SceneFocused = true, m_SceneHovered = true;
	};

}
