#pragma once

#include <entt.hpp>
#include "Engine/Core/Timestep.h"

namespace Engine::System::Physics {

	void OnUpdateKinematic(entt::registry& registry, Timestep ts);
	void OnUpdate(entt::registry& registry);

}