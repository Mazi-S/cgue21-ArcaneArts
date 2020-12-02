#pragma once
#include "Engine.h"

class ExampleLayer : public Engine::Layer {
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Engine::Timestep ts) override;

	virtual void OnEvent(Engine::Event& event) override;

	bool OnKeyPressed(Engine::KeyPressedEvent& event);

	void CreateMagicBall();

	// todo: remove (use MeshLibrary)
	Engine::Ref<Engine::Mesh> m_Sphere;

private:
	Engine::Ref<Engine::Scene> m_Scene;
};