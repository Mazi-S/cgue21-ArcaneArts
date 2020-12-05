#pragma once

#include <entt.hpp>

namespace Engine::System::Hero {
	void Throw(entt::registry& registry, entt::entity hero, bool rightHand = true);
}