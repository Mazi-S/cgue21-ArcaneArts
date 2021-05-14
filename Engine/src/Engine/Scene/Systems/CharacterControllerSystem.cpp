#include "egpch.h"
#include "CharacterControllerSystem.h"

#include "Engine/Scene/Components.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Application.h"
#include "Engine/Events/KeyEvent.h"

namespace Engine::System::CharacterController {

	void OnUpdate(entt::registry& registry, Timestep ts)
	{
		auto view = registry.view<Component::Physics::CharacterControllerComponent, Component::Core::TransformComponent>();
		for (const entt::entity e : view)
		{
			auto& [ccc, tc] = view.get<Component::Physics::CharacterControllerComponent, Component::Core::TransformComponent>(e);
			auto [currentMouseX, currentMouseY] = Engine::Input::GetMousePosition();

			if (ccc.Active)
			{
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
					float jump = 8 * ts;
					move += glm::vec4{ 0.0f, jump * ccc.Jump, 0.0f, 0.0f };
					ccc.Jump = glm::max(ccc.Jump - jump, -2.0f);
				}
				physx::PxControllerCollisionFlags collisionFlags = ccc.Controller->move({ move.x, move.y, move.z } , 0.001f, ts, physx::PxControllerFilters());

				// mouse
				tc.Rotation.x -= (currentMouseY - ccc.MouseY) * (ccc.RotationSpeed);
				tc.Rotation.x = glm::min(glm::half_pi<float>() - glm::epsilon<float>(), tc.Rotation.x);
				tc.Rotation.x = glm::max(-glm::half_pi<float>() + glm::epsilon<float>(), tc.Rotation.x);

				tc.Rotation.y -= (currentMouseX - ccc.MouseX) * (ccc.RotationSpeed);
			}

			ccc.MouseX = currentMouseX;
			ccc.MouseY = currentMouseY;
		}
	}

	bool OnKeyPressed(entt::registry& registry, Engine::KeyPressedEvent& event)
	{
		auto view = registry.view<Component::Physics::CharacterControllerComponent, Component::Core::TransformComponent>();
		for (const entt::entity e : view)
		{
			if (event.GetKeyCode() == Engine::Key::Space)
			{
				auto& ccc = view.get<Component::Physics::CharacterControllerComponent>(e);
				TryJump(registry, e);
			}
		}

		return false;
	}

	void Crouch(entt::registry& registry, entt::entity character)
	{
		Component::Physics::CharacterControllerComponent& ccc = registry.get<Component::Physics::CharacterControllerComponent>(character);
		ccc.Controller->resize(ccc.CrouchingHeight - 2.0f * ccc.Radius);
		
		// reposition camera
		auto view = registry.view<Component::Renderer::CameraComponent, Component::Core::ParentComponent, Component::Core::TransformComponent>();
		for (const entt::entity e : view)
		{
			auto& [cc, pc, tc] = view.get<Component::Renderer::CameraComponent, Component::Core::ParentComponent, Component::Core::TransformComponent>(e);
			if (pc.Parent == character)
			{
				tc.Translation.y *= ccc.CrouchingHeight / ccc.StandingHeight;
			}
		}
	}

	bool TryStandup(entt::registry& registry, entt::entity character)
	{
		Component::Physics::CharacterControllerComponent& ccc = registry.get<Component::Physics::CharacterControllerComponent>(character);
		
		float dh = ccc.StandingHeight - ccc.CrouchingHeight - 2.0f * ccc.Radius;
		physx::PxCapsuleGeometry geom(ccc.Radius, dh * 0.5f);

		physx::PxExtendedVec3 position = ccc.Controller->getPosition();
		physx::PxVec3 pos((float)position.x, (float)position.y + ccc.StandingHeight * .5f + ccc.Radius, (float)position.z);
		physx::PxQuat orientation(physx::PxHalfPi, physx::PxVec3(0.0f, 0.0f, 1.0f));

		physx::PxOverlapBuffer hit;

		if (ccc.Controller->getScene()->overlap(geom, physx::PxTransform(pos, orientation), hit, 
			physx::PxQueryFilterData(physx::PxQueryFlag::eANY_HIT | physx::PxQueryFlag::eSTATIC | physx::PxQueryFlag::eDYNAMIC)))
			return false;

		ccc.Controller->resize(ccc.StandingHeight - 2.0f * ccc.Radius);

		// reposition camera
		auto view = registry.view<Component::Renderer::CameraComponent, Component::Core::ParentComponent, Component::Core::TransformComponent>();
		for (const entt::entity e : view)
		{
			auto& [cc, pc, tc] = view.get<Component::Renderer::CameraComponent, Component::Core::ParentComponent, Component::Core::TransformComponent>(e);
			if (pc.Parent == character)
			{
				tc.Translation.y *= ccc.StandingHeight / ccc.CrouchingHeight;
			}
		}

		return true;
	}

	bool TryJump(entt::registry& registry, entt::entity character)
	{
		auto& ccc = registry.get<Component::Physics::CharacterControllerComponent>(character);

		physx::PxScene* scene = ccc.Controller->getScene();
		float halfHeight = (ccc.Crouching ? ccc.CrouchingHeight : ccc.StandingHeight) * 0.5f;
		physx::PxVec3 origin = physx::toVec3(ccc.Controller->getPosition());
		physx::PxVec3 unitDir = { 0.0f, -1.0f, 0.0f };
		float maxDist = halfHeight + 0.11f;

		const uint32_t bufferSize = 256;
		physx::PxRaycastHit hitBuffer[bufferSize];
		physx::PxRaycastBuffer buf(hitBuffer, bufferSize);

		bool status = scene->raycast(origin, unitDir, maxDist, buf);
		if (buf.nbTouches < 2)
			return false;

		ccc.Jump = 2;
		return true;
	}

}