#pragma once
#include "Engine/Events/GameEvent.h"
#include "GameEventType.h"

class CharacterHealthEvent : public Engine::GameEvent {
public:
	CharacterHealthEvent(float health)
		: m_Health(health) { }

	float GetHealth() { return m_Health; }

	virtual GameEventType GetGameEventType() const override { return GetStaticType(); }
	static GameEventType GetStaticType() { return GameEventType::CharacterHealth; }

private:
	float m_Health;
};
