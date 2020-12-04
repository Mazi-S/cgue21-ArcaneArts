#pragma once

#include "entt.hpp"

#include "Engine/Core/Timestep.h"
#include "Engine/Events/Event.h"

#include <glm/glm.hpp>

#include "Engine/Renderer/SceneCamera.h"
#include "Engine/Renderer/CameraController.h"

namespace Engine {

	class Scene
	{
		friend class Entity;

	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep ts);
		void OnRender();
		void OnEvent(Event& event);

		void OnViewportResize(uint32_t width, uint32_t height);

		std::pair<uint32_t, uint32_t> GetVieportSize();

	private:
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		entt::registry m_Registry;

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		bool m_SceneFocused = true, m_SceneHovered = true;
	};

}
