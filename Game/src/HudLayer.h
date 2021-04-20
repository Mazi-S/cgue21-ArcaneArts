#pragma once
#include "Engine.h"
#include "Events/CharacterHealthEvent.h"
#include "Events/CharacterManaEvent.h"
#include "Events/MonsterDied.h"
#include "Events/GameEnd.h"

struct HealthBar
{
	Engine::Entity Bar;
	Engine::Entity Background;
	Engine::Entity Health;

	float Width = 300.0f;
	float Height = 15.0f;
	float Offset = 5;
	
	void Init(entt::registry& registry);
	void UpdateHealth(float hp);
	void UpdatePosition(glm::vec2 position);
};

struct ManaBar
{
	Engine::Entity Bar;
	Engine::Entity Background;
	Engine::Entity Mana;

	float Width = 300.0f;
	float Height = 15.0f;
	float OffsetTop = 5;
	float OffsetLeft = 20;

	void Init(entt::registry& registry);
	void UpdateMana(float m);
	void UpdatePosition(glm::vec2 position);
};

struct MonsterDisplay
{
	Engine::Entity Bar;
	std::vector<Engine::Entity> Monsters;

	Engine::Ref<Engine::OpenGL::GlTexture2D> Sprite;

	float OffsetTop = 5;
	float OffsetRight = 5;
	float OffsetMonster = 5;

	glm::vec2 Size = { 30, 30 };

	void Init(entt::registry& registry, Engine::Ref<Engine::OpenGL::GlTexture2D> sprite, uint32_t count);
	void Add();
	void Remove();
	void UpdatePosition(glm::vec2 position);

private:
	entt::registry* Registry;
};


class HudLayer : public Engine::Layer {
public:
	HudLayer();
	virtual ~HudLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Engine::Timestep ts) override;

	virtual void OnEvent(Engine::Event& event) override;

	bool OnWindowResize(Engine::WindowResizeEvent& event);
	bool OnKeyPressed(Engine::KeyPressedEvent& event);
	bool OnHealthChange(CharacterHealthEvent& event);
	bool OnMonsterDied(MonsterDiedEvent& event);
	bool OnManaChange(CharacterManaEvent& event);
	bool OnGameEnd(GameEndEvent& event);

private:
	bool m_HUD;

	entt::registry m_Registry;

	Engine::Camera m_Camera;

	HealthBar m_HealthBar;
	ManaBar m_ManaBar;
	MonsterDisplay m_MonsterDisplay;
};