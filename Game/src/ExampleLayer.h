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

private:

	Engine::Ref<Engine::VertexArray> m_VA;
	Engine::Ref<Engine::Shader> m_Shader;
	Engine::Ref<Engine::Texture> m_Texture;
	
	Engine::Ref<Engine::SceneCamera> m_Camera;
	Engine::Ref<Engine::CameraController> m_CameraController;
	
};