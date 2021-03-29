#pragma once
#include "Engine/Events/GameEvent.h"
#include "GameEventType.h"

class CharacterManaEvent : public Engine::GameEvent {
public:
	CharacterManaEvent(float mana)
		: m_Mana(mana) { }

	float GetMana() { return m_Mana; }

	virtual GameEventType GetGameEventType() const override { return GetStaticType(); }
	static GameEventType GetStaticType() { return GameEventType::CharacterMana; }

private:
	float m_Mana;
};
