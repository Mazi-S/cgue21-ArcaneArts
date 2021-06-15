#pragma once
#include "Engine.h"

class PointLightFlickerScript : public Engine::ScriptableEntity
{
public:
	PointLightFlickerScript(Engine::Entity entity) : ScriptableEntity(entity)
	{ }

	virtual void OnUpdate(Engine::Timestep ts) override;

};

