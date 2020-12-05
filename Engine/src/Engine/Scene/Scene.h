#pragma once

#include "entt.hpp"

#include "Engine/Core/Timestep.h"
#include "Engine/Events/Event.h"
#include "Engine/Core/Application.h"

#include <glm/glm.hpp>

namespace Engine {

	class Scene
	{
		friend class Entity;

	public:
		Scene();
		~Scene();

		// ECS
		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateHero();
		Entity CreateMainCamera(Entity parent);
		void DestroyEntity(entt::entity entity);

		Entity CreateMagicBall(bool rightHand);
		void Throw(bool rightHand = true);

		void OnUpdate(Timestep ts);
		void OnRender();
		void OnEvent(Event& event);

		void OnViewportResize(uint32_t width, uint32_t height);

		std::pair<uint32_t, uint32_t> GetVieportSize();

	private:
		bool OnWindowResize(WindowResizeEvent& e);

		void InitCameraComponent(entt::registry& registry, entt::entity entity);

	private:
		entt::registry m_Registry;
		entt::entity m_Hero = entt::null;
		entt::entity m_MainCamera = entt::null;

		uint32_t m_ViewportWidth, m_ViewportHeight;

		bool m_SceneFocused = true, m_SceneHovered = true;
	};

}
