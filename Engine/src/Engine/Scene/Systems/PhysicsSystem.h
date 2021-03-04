#pragma once

#include <entt.hpp>
#include "Engine/Core/Timestep.h"

namespace Engine::System::Physics {

	void OnUpdate(entt::registry& registry);
	void OnUpdateKinematic(entt::registry& registry, Timestep ts);
}