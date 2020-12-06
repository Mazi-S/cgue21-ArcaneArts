#pragma once

#include <entt.hpp>
#include <glm/glm.hpp>

#include "Engine/Scene/Components.h"

namespace Engine::System::Util {

	glm::mat4 Transform(const TransformComponent& tc);
	glm::mat4 Transform(entt::registry& registry, entt::entity entity);
	TransformComponent Combine(const TransformComponent& first, const TransformComponent& second);

	void RecalculateProjection(CameraComponent& cc);
}