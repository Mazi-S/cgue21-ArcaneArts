#pragma once

#include "Engine.h"

enum class MagicBallType
{
	Light,
	Fire,
	Lightning,
};

class MagicBall : public Engine::ScriptableEntity
{
public:
	MagicBall(Engine::Entity entity)
		: ScriptableEntity(entity)
	{ }

	virtual void OnUpdate(Engine::Timestep ts) override;

};
