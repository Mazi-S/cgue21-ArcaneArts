#pragma once
#include "Engine.h"

class PhysicsTestLayer : public Engine::Layer {
public:
	PhysicsTestLayer();
	virtual ~PhysicsTestLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Engine::Timestep ts) override;

	virtual void OnEvent(Engine::Event& event) override;

	bool OnKeyPressed(Engine::KeyPressedEvent& event);

private:
	void InitScene();

private:
	Engine::Ref<Engine::Scene> m_Scene;
	Engine::Entity m_Character;
};