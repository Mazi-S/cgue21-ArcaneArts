#pragma once

#include "Engine.h"

class MonsterBig : public Engine::ScriptableEntity
{
public:
	MonsterBig(Engine::Entity entity) : ScriptableEntity(entity)
	{ }

	virtual void OnUpdate(Engine::Timestep ts) override;

private:

	float m_Hitpoints = 100.0f;
};

