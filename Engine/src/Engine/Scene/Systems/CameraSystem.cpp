#include "egpch.h"
#include "CameraSystem.h"

#include "Engine/Scene/Components.h"

#include "Util.h"

namespace Engine::System::Camera {

	Engine::Camera GetCamera(entt::registry& registry)
	{
		auto view = registry.view<CameraComponent, TransformComponent>();
		for (const entt::entity e : view)
		{
			auto& cc = view.get<CameraComponent>(e);
			return Engine::Camera(cc.Projection, Util::Transform(registry, e));
		}
		return Engine::Camera();
	}

	Engine::Camera GetCamera(entt::registry& registry, entt::entity camera)
	{
		auto* cc = registry.try_get<CameraComponent>(camera);
		if (cc != nullptr)
			return Engine::Camera(cc->Projection, Util::Transform(registry, camera));

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
