#pragma once
#include "Engine.h"

enum class MagicBallType
{
	None = 0,
	Light,
	Heal,
	Fire,
	Lightning,
};

float MagicBallMana(MagicBallType type);

float MagicBallCastTime(MagicBallType type);

float MagicBallEffect(MagicBallType type);

class MagicBallScript : public Engine::ScriptableEntity
{
public:
	MagicBallScript(Engine::Entity entity)
		: ScriptableEntity(entity)
	{ }

	virtual void OnUpdate(Engine::Timestep ts) override;
};
