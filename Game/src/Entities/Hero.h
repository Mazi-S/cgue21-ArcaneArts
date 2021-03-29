#pragma once

#include "Engine.h"
#include "MagicBall.h"

class Hero : public Engine::ScriptableEntity
{
public:
	Hero(Engine::Entity entity) : ScriptableEntity(entity)
	{ }

	virtual void OnEvent(Engine::Event& e) override;

private:
	bool OnMouseButtonPressed(Engine::MouseButtonPressedEvent& e);
	bool OnMouseScrolled(Engine::MouseScrolledEvent& e);
	bool OnKeyPressed(Engine::KeyPressedEvent& e);
	void UseLeftHand();
	void UseRightHand();

	Engine::Entity CreateMagicBall(MagicBallType type, glm::vec3 offset);
	void Throw(Engine::Entity item);
	void Light(Engine::Entity ball);
	void Heal(Engine::Entity ball);
	void DropLeft();
	void DropRight();

private:
	MagicBallType m_ActiveSpell = MagicBallType::Fire;
	MagicBallType m_PassiveSpell = MagicBallType::Light;

	Engine::Entity m_LeftHand; // Passive Spells
	Engine::Entity m_RightHand; // Active Spells
};

