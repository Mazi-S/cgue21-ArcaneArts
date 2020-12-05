#pragma once

#include <entt.hpp>
#include "Engine/Core/Timestep.h"

namespace Engine::System ::CharacterController {

	void OnUpdate(entt::registry& registry, Timestep ts);
	void Activate(entt::registry& registry, entt::entity controller);

}