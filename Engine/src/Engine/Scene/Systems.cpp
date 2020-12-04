#include "egpch.h"

#include "Components.h"
#include "Systems.h"

#include "Engine/Scene/Systems/Util.h"

#include "Engine/Core/Input.h"
#include "Engine/Core/Assert.h"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

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

				if (Engine::Input::IsMouseButtonPressed(Engine::Mouse::ButtonLeft))
				{
					tc.Rotation.x -= (currentMouseY - ccc.MouseY) * (ccc.RotationSpeed);
					tc.Rotation.x = glm::min(glm::half_pi<float>() - glm::epsilon<float>(), tc.Rotation.x);
					tc.Rotation.x = glm::max(-glm::half_pi<float>() + glm::epsilon<float>(), tc.Rotation.x);
					
					tc.Rotation.y -= (currentMouseX - ccc.MouseX) * (ccc.RotationSpeed);
				}

				ccc.MouseX = currentMouseX;
				ccc.MouseY = currentMouseY;
			}
		}
	}

}

namespace Engine::System::Camera {

	Engine::Camera GetCamera(entt::registry& registry)
	{
		auto view = registry.view<CameraComponent, TransformComponent>();
		for (const entt::entity e : view)
		{
			auto& [cc, tc] = view.get<CameraComponent, TransformComponent>(e);
			return Engine::Camera(cc.Projection, Util::Transform(tc));
		}
		return Engine::Camera();
	}

	void SetViewportSize(entt::registry& registry, uint32_t width, uint32_t height)
	{
		auto view = registry.view<CameraComponent>();
		for (const entt::entity e : view)
		{
			auto& cc = view.get<CameraComponent>(e);
			cc.AspectRatio = (float)width / (float)height;
			Util::RecalculateProjection(cc);
		}
	}
}


