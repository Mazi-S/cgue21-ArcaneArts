#pragma once
#include "Engine.h"

class ItemScript : public Engine::ScriptableEntity
{
public:
	ItemScript(Engine::Entity entity) : ScriptableEntity(entity)
	{ }

	virtual void OnUpdate(Engine::Timestep ts) override;

};
