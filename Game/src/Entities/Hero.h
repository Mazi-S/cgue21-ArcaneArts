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
	void UseLeftHand();
	void UseRightHand();

	void CreateMagicBall(MagicBallType type);
	void ThrowLeft();
	void ThrowRight();
	void DropLeft();
	void DropRight();

private:
	MagicBallType m_AktiveSpell = MagicBallType::Fire;

	Engine::Entity m_LeftHand;
	Engine::Entity m_RightHand;
};

