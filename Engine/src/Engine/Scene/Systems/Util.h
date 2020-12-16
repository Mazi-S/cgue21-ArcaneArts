#pragma once

#include <entt.hpp>
#include <glm/glm.hpp>

#include "Engine/Scene/Components.h"

namespace Engine::System::Util {

	glm::mat4 Transform(const TransformComponent& tc);
	glm::vec3 Transform(const TransformComponent& tc, glm::vec3 v3);
	glm::mat4 Transform(entt::registry& registry, entt::entity entity);

	glm::vec3 Position(entt::registry& registry, entt::entity entity);

	void RecalculateProjection(CameraComponent& cc);
	void Activate(CharacterControllerComponent& ccc);
	void Deactivate(CharacterControllerComponent& ccc);
}