#pragma once

#include "Engine.h"

class Monster : public Engine::ScriptableEntity
{
public:
	Monster(Engine::Entity entity) : ScriptableEntity(entity)
	{ }

	virtual void OnUpdate(Engine::Timestep ts) override;

private:

	float m_Hitpoints = 100.0f;
};
