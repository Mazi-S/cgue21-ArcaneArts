#pragma once

#include <entt.hpp>

namespace Engine::System::Hero {

	enum class MagicBallType
	{
		Fire,
		Water,
		Light
	};

	void CreateMagicBall(entt::registry registry, entt::entity hero, MagicBallType type);
	void ThrowLeft(entt::registry& registry, entt::entity hero);
	void ThrowRight(entt::registry& registry, entt::entity hero);
}