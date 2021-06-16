#pragma once
#include "Engine.h"

class WalkingSoundScript : public Engine::ScriptableEntity
{
public:
	WalkingSoundScript(Engine::Entity entity) : ScriptableEntity(entity)
	{ }

	virtual void OnCreate() override;
	virtual void OnDestroy() override;

	virtual void OnUpdate(Engine::Timestep ts) override;

private:
	irrklang::ISound* m_Sound = nullptr;

};
