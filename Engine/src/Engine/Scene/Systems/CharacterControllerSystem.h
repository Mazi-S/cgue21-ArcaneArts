#pragma once

#include <entt.hpp>
#include "Engine/Core/Timestep.h"
#include "Engine/Events/KeyEvent.h"

namespace Engine::System::CharacterController {

	void OnUpdate(entt::registry& registry, Timestep ts);
	bool OnKeyPressed(entt::registry& registry, Engine::KeyPressedEvent& event);

	void Crouch(entt::registry& registry, entt::entity character);
	bool TryStandup(entt::registry& registry, entt::entity character);
}