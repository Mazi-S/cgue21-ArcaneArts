#pragma once

#include "Engine.h"

class MonsterSmall : public Engine::ScriptableEntity
{
public:
	MonsterSmall(Engine::Entity entity) : ScriptableEntity(entity)
	{ }

	virtual void OnUpdate(Engine::Timestep ts) override;

private:

	float m_Hitpoints = 40.0f;
};

