#pragma once
#include "Engine.h"
#include "Events/CharacterHealthEvent.h"

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

class HudLayer : public Engine::Layer {
public:
	HudLayer();
	virtual ~HudLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Engine::Timestep ts) override;

	virtual void OnEvent(Engine::Event& event) override;

	bool OnWindowResize(Engine::WindowResizeEvent& event);
	bool OnHealthChange(CharacterHealthEvent& event);

private:
	entt::registry m_Registry;

	Engine::Camera m_Camera;

	HealthBar m_HealthBar;
};