#pragma once
#include "Engine/Events/GameEvent.h"
#include "GameEventType.h"

class GameEndEvent : public Engine::GameEvent {
public:
	GameEndEvent(bool victory)
		: m_Victory(victory) { }

	bool Victory() { return m_Victory; }

	virtual GameEventType GetGameEventType() const override { return GetStaticType(); }
	static GameEventType GetStaticType() { return GameEventType::EndGame; }

private:
	bool m_Victory; 
};
