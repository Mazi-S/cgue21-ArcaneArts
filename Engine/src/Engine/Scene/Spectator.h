#pragma once
#include "Engine/Core/Timestep.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Renderer/Camera.h"

#include <glm/glm.hpp>

namespace Engine {

	class Spectator
	{
	public:

		Spectator();
		~Spectator() = default;

		void OnUpdate(Timestep ts);
		void OnEvent(Event& event);
		void SetViewportSize(uint32_t width, uint32_t height);

		Camera GetCamera();
		glm::vec3 GetPosition() { return m_Translation; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		void RecalculateProjection();

	private:
		// Camera
		float m_FOV = glm::radians(45.0f);
		float m_Near = 0.01f, m_Far = 1000.0f;

		float m_AspectRatio = 1.0f;
		glm::mat4 m_Projection = glm::mat4(1.0f);

		// Controller
		glm::mat4 m_Transform{ 1.0f };

		float m_TranslationSpeed = 7.0f;
		float m_RotationSpeed = 0.0005f;
		float m_ZoomSpeed = 1.0f;
		float m_MouseX = 0, m_MouseY = 0;
		float m_Zoom = 10.0f;
		glm::vec3 m_EulerAngles{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Translation{ 0.0f, 0.0f, 0.0f };
	};

}