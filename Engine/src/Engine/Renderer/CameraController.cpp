#include "egpch.h"
#include "CameraController.h"

#include "Engine/Core/Input.h"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Engine {

	static void LogKeyBindings()
	{
		// Movement
		//	Forward: W
		//	Left:    A
		//	Back:    S
		//	Right:   D
		//	Up:      Space
		//	Down:    LCtrl

		//	Zoom:    Scroll
		//	V FOV:   LAlt + Scroll

		LOG_INFO("CameraController: Key Bindings\nMovement\n  Forward: W\n  Left:    A\n  Back:    S\n  Right:   D\n  Up:      Space\n  Down:    LCtrl\n\n  Zoom:    Scroll\n  V FOV:   LAlt + Scroll");
	}

	CameraController::CameraController()
	{
		LogKeyBindings();
	}

	void CameraController::OnUpdate(Timestep ts)
	{
		auto [currentMouseX, currentMouseY] = Engine::Input::GetMousePosition();
		glm::vec4 move = { 0.0f, 0.0f, 0.0f, 1.0 };
		if (Engine::Input::IsKeyPressed(Engine::Key::D))
			move.x += m_TranslationSpeed * ts;
		if (Engine::Input::IsKeyPressed(Engine::Key::A))
			move.x -= m_TranslationSpeed * ts;
		if (Engine::Input::IsKeyPressed(Engine::Key::S))
			move.z += m_TranslationSpeed * ts;
		if (Engine::Input::IsKeyPressed(Engine::Key::W))
			move.z -= m_TranslationSpeed * ts;
		if (Engine::Input::IsKeyPressed(Engine::Key::Space))
			move.y += m_TranslationSpeed * ts;
		if (Engine::Input::IsKeyPressed(Engine::Key::LeftControl))
			move.y -= m_TranslationSpeed * ts;

		move = glm::toMat4(glm::quat({ 0.0f, m_EulerAngles.y, 0.0f })) * move;
		m_Translation.x += move.x; m_Translation.y += move.y; m_Translation.z += move.z;

		if (Engine::Input::IsMouseButtonPressed(Engine::Mouse::ButtonLeft))
		{
			m_EulerAngles.x -= (currentMouseY - m_MouseY) * (m_RotationSpeed + m_Zoom * 0.0004f);

			m_EulerAngles.x = glm::min(glm::half_pi<float>() - glm::epsilon<float>(), m_EulerAngles.x);
			m_EulerAngles.x = glm::max(-glm::half_pi<float>() + glm::epsilon<float>(), m_EulerAngles.x);

			m_EulerAngles.y -= (currentMouseX - m_MouseX) * (m_RotationSpeed + m_Zoom * 0.0004f);
		}

		temp_Transform = glm::translate(glm::mat4(1.0f), m_Translation) * glm::toMat4(glm::quat(m_EulerAngles)) * glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, m_Zoom });

		m_MouseX = currentMouseX;
		m_MouseY = currentMouseY;
	}

	void CameraController::OnEvent(Engine::Event& event)
	{
		Engine::EventHandler eventHandler(event);
		eventHandler.Handle<MouseScrolledEvent>(EG_BIND_EVENT_FN(CameraController::OnMouseScrolled));
		eventHandler.Handle<WindowResizeEvent>(EG_BIND_EVENT_FN(CameraController::OnWindowResize));
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		// Vertical FOV:  LAlt + Scroll
		if (Engine::Input::IsKeyPressed(Key::LeftAlt))
		{
			float fov = temp_Camera->GetVerticalFOV();
			fov -= e.GetYOffset() * 0.1f;
			fov = std::max(fov, glm::quarter_pi<float>() * 0.5f);
			fov = std::min(fov, glm::half_pi<float>());
			temp_Camera->SetVerticalFOV(fov);
			return false;
		}

		// Zoom: Scroll
		m_Zoom -= e.GetYOffset() * m_ZoomSpeed;
		m_Zoom = std::max(m_Zoom, 1.0f);
		m_Zoom = std::min(m_Zoom, 25.0f);

		return false;
	}

	bool CameraController::OnWindowResize(WindowResizeEvent& e)
	{
		temp_Camera->SetViewportSize(e.GetWidth(), e.GetHeight());
		return false;
	}
}