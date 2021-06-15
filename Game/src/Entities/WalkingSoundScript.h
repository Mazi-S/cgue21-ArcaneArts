#pragma once
#include "Engine.h"

class WalkingSoundScript : public Engine::ScriptableEntity
{
public:
	WalkingSoundScript(Engine::Entity entity) : ScriptableEntity(entity)
	{ }

	virtual void OnUpdate(Engine::Timestep ts) override;

};
