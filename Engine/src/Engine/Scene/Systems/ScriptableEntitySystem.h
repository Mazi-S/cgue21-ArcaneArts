#pragma once
#include <entt.hpp>
#include "Engine/Core/Timestep.h"

namespace Engine::System::ScriptableEntity {

	void OnUpdate(entt::registry& registry, Timestep ts);

}