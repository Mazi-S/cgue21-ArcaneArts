#include "egpch.h"
#include "Spectator.h"
#include "Engine/Core/Input.h"

#include <glm/gtx/quaternion.hpp>

namespace Engine {

	Spectator::Spectator()
	{
		RecalculateProjection();
	}

	void Spectator::OnUpdate(Timestep ts)
	{
		auto [currentMouseX, currentMouseY] = Input::GetMousePosition();

		glm::vec4 move = { 0.0f, 0.0f, 0.0f, 1.0 };
		if (Input::IsKeyPressed(Key::D))
			move.x += m_TranslationSpeed * ts;
		if (Input::IsKeyPressed(Key::A))
			move.x -= m_TranslationSpeed * ts;
		if (Input::IsKeyPressed(Key::S))
			move.z += m_TranslationSpeed * ts;
		if (Input::IsKeyPressed(Key::W))
			move.z -= m_TranslationSpeed * ts;
		if (Input::IsKeyPressed(Key::PageUp))
			move.y += m_TranslationSpeed * ts;
		if (Input::IsKeyPressed(Key::PageDown))
			move.y -= m_TranslationSpeed * ts;

		move = glm::toMat4(glm::quat({ 0.0f, m_EulerAngles.y, 0.0f })) * move;
		m_Translation.x += move.x; m_Translation.y += move.y; m_Translation.z += move.z;

		if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
		{
			m_EulerAngles.x -= (currentMouseY - m_MouseY) * (m_RotationSpeed + m_Zoom * 0.0004f);
			m_EulerAngles.x = glm::min(glm::half_pi<float>() - glm::epsilon<float>(), m_EulerAngles.x);
			m_EulerAngles.x = glm::max(-glm::half_pi<float>() + glm::epsilon<float>(), m_EulerAngles.x);

			m_EulerAngles.y -= (currentMouseX - m_MouseX) * (m_RotationSpeed + m_Zoom * 0.0004f);
		}

		m_Transform = glm::translate(glm::mat4(1.0f), m_Translation) * glm::toMat4(glm::quat(m_EulerAngles)) * glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, m_Zoom });

		m_MouseX = currentMouseX;
		m_MouseY = currentMouseY;
	}

	void Spectator::OnEvent(Event& event)
	{
		EventHandler eventDispatcher(event);
		eventDispatcher.Handle<MouseScrolledEvent>(EG_BIND_EVENT_FN(Spectator::OnMouseScrolled));
	}

	void Spectator::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	Camera Spectator::GetCamera()
	{
		return Camera(m_Projection, m_Transform);
	}

	bool Spectator::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_Zoom -= e.GetYOffset() * m_ZoomSpeed;
		m_Zoom = std::max(m_Zoom, 1.0f);
		m_Zoom = std::min(m_Zoom, 25.0f);
		return true;
	}

	void Spectator::RecalculateProjection()
	{
		m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_Near, m_Far);
	}

}