#pragma once
#include "Engine/Events/GameEvent.h"
#include "GameEventType.h"

class MenuEvent : public Engine::GameEvent {
public:
	MenuEvent(bool open)
		: m_Open(open) { }

	bool Open() { return m_Open; }

	virtual GameEventType GetGameEventType() const override { return GetStaticType(); }
	static GameEventType GetStaticType() { return GameEventType::Menu; }

private:
	bool m_Open;
};
