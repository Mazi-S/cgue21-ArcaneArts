#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Core/Timestep.h"

#include "Engine/Renderer/SceneCamera.h"

#include "Engine/Events/Event.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/ApplicationEvent.h"

#include <glm/glm.hpp>

namespace Engine {

	class CameraController
	{
	public:
		CameraController();
		~CameraController() = default;

		void OnUpdate(Timestep ts);
		void OnEvent(Engine::Event& event);

		// todo: move to tranform component
		glm::mat4 temp_Transform{1.0f};
		Ref<SceneCamera> temp_Camera;

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		float m_TranslationSpeed = 7.0f;
		float m_RotationSpeed = 0.0005f;
		float m_ZoomSpeed = 1.0f;
		float m_MouseX = 0, m_MouseY = 0;
		float m_Zoom = 10.0f;
		glm::vec3 m_EulerAngles{0.0f, 0.0f, 0.0f};
		glm::vec3 m_Translation{0.0f, 0.0f, 0.0f};
	};

}
