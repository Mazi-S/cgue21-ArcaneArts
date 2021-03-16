#include "egpch.h"
#include "CameraSystem.h"

#include "Engine/Scene/Components.h"

#include "Util.h"

namespace Engine::System::Camera {

	Engine::Camera GetCamera(entt::registry& registry)
	{
		auto view = registry.view<Component::Renderer::CameraComponent, Component::Core::TransformComponent>();
		for (const entt::entity e : view)
		{
			auto& cc = view.get<Component::Renderer::CameraComponent>(e);
			return Engine::Camera(cc.Projection, Util::Transform(registry, e));
		}
		return Engine::Camera();
	}

	Engine::Camera GetCamera(entt::registry& registry, entt::entity camera)
	{
		auto* cc = registry.try_get<Component::Renderer::CameraComponent>(camera);
		if (cc != nullptr)
			return Engine::Camera(cc->Projection, Util::Transform(registry, camera));

		return Engine::Camera();
	}

	void SetViewportSize(entt::registry& registry, uint32_t width, uint32_t height)
	{
		auto view = registry.view<Component::Renderer::CameraComponent>();
		for (const entt::entity e : view)
		{
			auto& cc = view.get<Component::Renderer::CameraComponent>(e);
			cc.AspectRatio = (float)width / (float)height;
			Util::RecalculateProjection(cc);
		}
	}

	void SetViewportSize(entt::registry& registry, entt::entity entity, uint32_t width, uint32_t height)
	{
		auto& cc = registry.get<Component::Renderer::CameraComponent>(entity);
		cc.AspectRatio = (float)width / (float)height;
		Util::RecalculateProjection(cc);
	}
}
