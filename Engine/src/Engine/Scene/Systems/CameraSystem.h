#pragma once

#include <entt.hpp>

#include "Engine/Renderer/Camera.h"

namespace Engine::System::Camera {
	Engine::Camera GetCamera(entt::registry& registry);
	Engine::Camera GetCamera(entt::registry& registry, entt::entity camera);

	void SetViewportSize(entt::registry& registry, uint32_t width, uint32_t height);
	void SetViewportSize(entt::registry& registry, entt::entity entity, uint32_t width, uint32_t height);
}