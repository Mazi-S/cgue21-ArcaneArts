#pragma once

#include <entt.hpp>
#include <glm/glm.hpp>

#include "Engine/Scene/Components.h"

namespace Engine::System::Util {

	void MakeIndependent(entt::registry& registry, entt::entity entity);

	glm::mat4 Transform(const Component::Core::TransformComponent& tc);
	glm::vec3 Transform(const Component::Core::TransformComponent& tc, glm::vec3 v3);

	glm::mat4 GlobalTransformMatrix(entt::registry& registry, entt::entity entity);
	Component::Core::TransformComponent GlobalTransform(entt::registry& registry, entt::entity entity);

	glm::vec3 Position(entt::registry& registry, entt::entity entity);

	void RecalculateProjection(Component::Renderer::CameraComponent& cc);
}