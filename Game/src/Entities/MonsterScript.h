#pragma once

#include "Engine.h"

class MonsterScript : public Engine::ScriptableEntity
{
public:
	MonsterScript(Engine::Entity entity) : ScriptableEntity(entity)
	{ }

	virtual void OnUpdate(Engine::Timestep ts) override;

};

