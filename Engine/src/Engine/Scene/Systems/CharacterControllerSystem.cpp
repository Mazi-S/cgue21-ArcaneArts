#include "egpch.h"
#include "CharacterControllerSystem.h"

#include "Engine/Scene/Components.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Application.h"

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

				// mouse
				tc.Rotation.x -= (currentMouseY - ccc.MouseY) * (ccc.RotationSpeed);
				tc.Rotation.x = glm::min(glm::half_pi<float>() - glm::epsilon<float>(), tc.Rotation.x);
				tc.Rotation.x = glm::max(-glm::half_pi<float>() + glm::epsilon<float>(), tc.Rotation.x);

				tc.Rotation.y -= (currentMouseX - ccc.MouseX) * (ccc.RotationSpeed);

				Application::Get().GetWindow().SetCursorPosition(ccc.MouseX, ccc.MouseY);
			}
		}
	}

}