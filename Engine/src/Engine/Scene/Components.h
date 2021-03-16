#pragma once

#include "Components/CoreComponents.h"
#include "Components/RendererComponents.h"
#include "Components/PhysicsComponents.h"
#include "Components/AudioComponents.h"
#include "Components/EventComponents.h"

namespace Engine {

	struct MonsterComponent
	{
		uint16_t temp = 0; // todo: fix

		MonsterComponent() = default;
		MonsterComponent(const MonsterComponent&) = default;
	};

	struct MagicBallHitComponent
	{
		uint16_t temp = 0; // todo: fix

		MagicBallHitComponent() = default;
		MagicBallHitComponent(const MagicBallHitComponent&) = default;
	};

}