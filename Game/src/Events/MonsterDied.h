#pragma once
#include "Engine/Events/GameEvent.h"
#include "GameEventType.h"

class MonsterDiedEvent : public Engine::GameEvent {
public:
	MonsterDiedEvent() = default;

	virtual GameEventType GetGameEventType() const override { return GetStaticType(); }
	static GameEventType GetStaticType() { return GameEventType::MonsterDied; }
};
