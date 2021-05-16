#pragma once

#include <entt.hpp>
#include "Engine/Scene/Entity.h"

namespace Engine::Factory {

	entt::entity CreateEntity(entt::registry& registry, const std::string& name = std::string());

	entt::entity CreateEntity(entt::registry& registry, const std::string& name, uint32_t id);

	entt::entity Copy(entt::registry& registry, entt::entity entity);
}
