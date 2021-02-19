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
				float speed = 1.0f;

				// crouching
				if (Engine::Input::IsKeyPressed(Engine::Key::LeftControl))
				{
					if (!ccc.Crouching)
					{
						Crouch(registry, e);
						ccc.Crouching = true;
					}

					speed = 0.5f;
				}
				else if (ccc.Crouching)
				{
					ccc.Crouching = !TryStandup(registry, e);
				}

				// running
				if (Engine::Input::IsKeyPressed(Engine::Key::LeftShift))
					speed = 2.0f;

				if (Engine::Input::IsKeyPressed(Engine::Key::D))
					move.x += ccc.TranslationSpeed * speed * ts;
				if (Engine::Input::IsKeyPressed(Engine::Key::A))
					move.x -= ccc.TranslationSpeed * speed * ts;
				if (Engine::Input::IsKeyPressed(Engine::Key::S))
					move.z += ccc.TranslationSpeed * speed * ts;
				if (Engine::Input::IsKeyPressed(Engine::Key::W))
					move.z -= ccc.TranslationSpeed * speed * ts;

				move = glm::toMat4(glm::quat({ 0.0f, tc.Rotation.y, 0.0f })) * move;

				{ // todo: improve
					float jump = ccc.JumpHeight * ts;
					move += glm::vec4{ 0.0f, jump * ccc.Jump, 0.0f, 0.0f };
					ccc.Jump = glm::max(ccc.Jump - jump, -2.0f);
				}
				physx::PxControllerCollisionFlags collisionFlags = ccc.Controller->move({ move.x, move.y, move.z } , 0.001f, ts, physx::PxControllerFilters());

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
				auto& ccc = view.get<CharacterControllerComponent>(e);
				ccc.Jump = ccc.JumpHeight;
			}

			if (event.GetKeyCode() == Engine::Key::LeftControl)
			{
				auto& ccc = view.get<CharacterControllerComponent>(e);
				ccc.Jump = -ccc.JumpHeight;
			}
		}

		return false;
	}

	void Crouch(entt::registry& registry, entt::entity character)
	{
		CharacterControllerComponent& ccc = registry.get<CharacterControllerComponent>(character);
		ccc.Controller->resize((ccc.Height - ccc.Radius * 2.0f) * ccc.CrouchHeight);
		
		auto view = registry.view<CameraComponent, ParentComponent, TransformComponent>();
		for (const entt::entity e : view)
		{
			auto& [cc, pc, tc] = view.get<CameraComponent, ParentComponent, TransformComponent>(e);
			if (pc.Parent == character)
			{
				tc.Translation.y *= ccc.CrouchHeight;
			}
		}
	}

	bool TryStandup(entt::registry& registry, entt::entity character)
	{
		CharacterControllerComponent& ccc = registry.get<CharacterControllerComponent>(character);
		// todo: check overlap
		if (false)
			return false;

		ccc.Controller->resize((ccc.Height - ccc.Radius * 2.0f));

		auto view = registry.view<CameraComponent, ParentComponent, TransformComponent>();
		for (const entt::entity e : view)
		{
			auto& [cc, pc, tc] = view.get<CameraComponent, ParentComponent, TransformComponent>(e);
			if (pc.Parent == character)
			{
				tc.Translation.y *= 1.0/ccc.CrouchHeight;
			}
		}

		return true;
	}

}