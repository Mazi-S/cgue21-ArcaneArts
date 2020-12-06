#pragma once

#include <entt.hpp>
#include "Engine/Scene/Entity.h"

namespace Engine::Factory {

		entt::entity CreateEntity(entt::registry& registry, const std::string& name = std::string());
		entt::entity CreateHero(entt::registry& registry);
		entt::entity CreateCamera(entt::registry& registry, entt::entity parent = entt::null, glm::vec3 offset = { 0.0f, 0.0f, 0.0f } , glm::vec3 rotation = { 0.0f, 0.0f, 0.0f });

		entt::entity CreateMagicBall(entt::registry& registry, entt::entity hero, bool mainHand = true);
}