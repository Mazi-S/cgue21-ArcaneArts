#pragma once

#include "Engine.h"

enum class MagicBallType
{
	Light,
	Fire,
	Water,
};

class MagicBall : public Engine::ScriptableEntity
{
public:
	MagicBall(Engine::Entity entity, float lifetime = 3.0f)
		: ScriptableEntity(entity), m_Lifetime(lifetime)
	{ }

	virtual void OnUpdate(Engine::Timestep ts) override;

private:
	float m_Lifetime;
};
