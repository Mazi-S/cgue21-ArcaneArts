#include "egpch.h"

#include "Components.h"
#include "Systems.h"

#include "Engine/Scene/Systems/Util.h"

#include "Engine/Renderer/Renderer.h"

#include "Engine/Core/Application.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Assert.h"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Engine::System {

	glm::mat4 GetTransform(entt::registry& registry, const entt::entity& entity)
	{
		glm::mat4 transform = glm::mat4(1.0f);

		auto* tc = registry.try_get<TransformComponent>(entity);
		if (tc != nullptr)
			transform = Util::Transform(*tc);

		auto* pc = registry.try_get<ParentComponent>(entity);
		if (pc != nullptr)
			transform = GetTransform(registry, pc->Parent) * transform;

		return transform;
	}

}


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

	void Activate(entt::registry& registry, entt::entity controller)
	{
		auto* ccc = registry.try_get<CharacterControllerComponent>(controller);

		if (ccc == nullptr)
			return;

		Application::Get().GetWindow().HideCursor();

		auto [currentMouseX, currentMouseY] = Engine::Input::GetMousePosition();
		ccc->MouseX = currentMouseX;
		ccc->MouseY = currentMouseY;

		ccc->Active = true;

	}

}

namespace Engine::System::Camera {

	Engine::Camera GetCamera(entt::registry& registry)
	{
		auto view = registry.view<CameraComponent, TransformComponent>();
		for (const entt::entity e : view)
		{
			auto& cc = view.get<CameraComponent>(e);
			return Engine::Camera(cc.Projection, GetTransform(registry, e));
		}
		return Engine::Camera();
	}

	Engine::Camera GetCamera(entt::registry& registry, entt::entity camera)
	{
		auto* cc = registry.try_get<CameraComponent>(camera);
		if(cc != nullptr)
			return Engine::Camera(cc->Projection, GetTransform(registry, camera));

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

	void SetViewportSize(entt::registry& registry, entt::entity entity, uint32_t width, uint32_t height)
	{
		auto& cc = registry.get<CameraComponent>(entity);
		cc.AspectRatio = (float)width / (float)height;
		Util::RecalculateProjection(cc);
	}
}

namespace Engine::System::Renderer {

	void Submit(entt::registry& registry)
	{
		auto view = registry.view<TransformComponent, MaterialComponent, MeshComponent>();
		for (auto entity : view)
		{
			auto [material, mesh] = view.get<MaterialComponent, MeshComponent>(entity);
			Engine::Renderer::Submit(mesh, material, GetTransform(registry, entity));
		}
	}

}

namespace Engine::System::Hero {


}