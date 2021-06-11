#pragma once

#include "Engine.h"
#include "MagicBallScript.h"

class HeroScript : public Engine::ScriptableEntity
{
public:
	HeroScript(Engine::Entity entity);

	virtual void OnUpdate(Engine::Timestep ts) override;
	virtual void OnEvent(Engine::Event& e) override;

private:
	bool OnMouseButtonPressed(Engine::MouseButtonPressedEvent& e);
	bool OnMouseButtonReleased(Engine::MouseButtonReleasedEvent& e);
	bool OnMouseScrolled(Engine::MouseScrolledEvent& e);
	bool OnKeyPressed(Engine::KeyPressedEvent& e);

	void UpdateMana(Engine::Timestep ts, bool restore);
	void UpdateHealth();

	void EquipActive(MagicBallType type);
	bool StartActive();
	bool UseActive(Engine::Timestep ts);
	bool StopActive();
	void CancelActive();

	void EquipPassive(MagicBallType type);
	bool StartPassive();
	bool UsePassive(Engine::Timestep ts);
	bool StopPassive();
	void CancelPassive();

	Engine::Entity CreateMagicBall(MagicBallType type, glm::vec3 offset);
	Engine::Entity CreateMagicBallParticleSystem(MagicBallType type, glm::vec3 offset);

	void Throw(Engine::Entity ball);

private:
	MagicBallType m_ActiveSpell = MagicBallType::None;
	MagicBallType m_PassiveSpell = MagicBallType::None;

	Engine::Entity m_PassiveHand;
	Engine::Entity m_ActiveHand;
	Engine::Entity m_ActiveParticleSystem;
};

