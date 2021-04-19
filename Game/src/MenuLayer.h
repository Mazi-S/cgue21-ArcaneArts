#pragma once
#include "Engine.h"

class MenuLayer : public Engine::Layer {
public:
	MenuLayer();
	virtual ~MenuLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Engine::Timestep ts) override;

	virtual void OnEvent(Engine::Event& event) override;
	bool OnKeyPressed(Engine::KeyPressedEvent& event);
	bool OnWindowResize(Engine::WindowResizeEvent& event);

private:
	
	bool m_Menu;

	Engine::Camera m_Camera;

	Engine::Ref<Engine::OpenGL::GlTexture2D> m_Controls;
};