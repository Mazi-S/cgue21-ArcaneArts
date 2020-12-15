#include "egpch.h"
#include "CharacterControllerSystem.h"

#include "Engine/Scene/Components.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Application.h"
#include "Engine/Events/KeyEvent.h"

namespace Engine::System::CharacterController {

	void OnUpdate(entt::registry& registry, Timestep ts)
	{
		auto view = registry.view<CharacterControllerComponent, TransformComponent>();
		for (const entt::entity e : view)
		{
			auto& [ccc, tc] = view.get<CharacterControllerComponent, TransformComponent>(e);
			if (ccc.Active)
			{
				auto [currentMouseX, currentMouseY] = Engine::Input::GetMousePosition();
				glm::vec4 move = { 0.0f, 0.0f, 0.0f, 1.0 };
				if (Engine::Input::IsKeyPressed(Engine::Key::D))
					move.x += ccc.TranslationSpeed * ts;
				if (Engine::Input::IsKeyPressed(Engine::Key::A))
					move.x -= ccc.TranslationSpeed * ts;
				if (Engine::Input::IsKeyPressed(Engine::Key::S))
					move.z += ccc.TranslationSpeed * ts;
				if (Engine::Input::IsKeyPressed(Engine::Key::W))
					move.z -= ccc.TranslationSpeed * ts;
				if (Engine::Input::IsKeyPressed(Engine::Key::Space))
					move.y += ccc.TranslationSpeed * ts;
				if (Engine::Input::IsKeyPressed(Engine::Key::LeftControl))
					move.y -= ccc.TranslationSpeed * ts;

				move = glm::toMat4(glm::quat({ 0.0f, tc.Rotation.y, 0.0f })) * move;
				tc.Translation.x += move.x;
				tc.Translation.y += move.y;
				tc.Translation.z += move.z;

				auto vc = registry.try_get<VelocityComponent>(e);
				if (vc != nullptr)
				{
					float gravity = -30.0f;
					vc->Velocity.y = std::max(-10.0f, vc->Velocity.y + gravity * ts);
					tc.Translation.y = std::max(tc.Translation.y, 0.0f);
				}

				// mouse
				tc.Rotation.x -= (currentMouseY - ccc.MouseY) * (ccc.RotationSpeed);
				tc.Rotation.x = glm::min(glm::half_pi<float>() - glm::epsilon<float>(), tc.Rotation.x);
				tc.Rotation.x = glm::max(-glm::half_pi<float>() + glm::epsilon<float>(), tc.Rotation.x);

				tc.Rotation.y -= (currentMouseX - ccc.MouseX) * (ccc.RotationSpeed);
				ccc.MouseX = currentMouseX;
				ccc.MouseY = currentMouseY;
			}
		}
	}

	bool OnKeyPressed(entt::registry& registry, Engine::KeyPressedEvent& event)
	{
		auto view = registry.view<CharacterControllerComponent, TransformComponent>();
		for (const entt::entity e : view)
		{
			if (event.GetKeyCode() == Engine::Key::Space)
			{
				registry.emplace_or_replace<VelocityComponent>(e, glm::vec3{ 0.0f, 10.0f, 0.0f });
			}
		}

		return false;
	}

}