#pragma once
#include "Engine.h"
#include "Events/GameEnd.h"

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
	bool OnGameEnd(GameEndEvent& event);

private:

	enum class GameState
	{
		Running,
		Victory,
		Defeat
	};
	
	bool m_Menu;
	bool m_Controls;
	GameState m_GameState;

	Engine::Camera m_Camera;

	Engine::Ref<Engine::OpenGL::GlTexture2D> m_ControlsTex;
	Engine::Ref<Engine::OpenGL::GlTexture2D> m_Victory;
	Engine::Ref<Engine::OpenGL::GlTexture2D> m_Defeat;
};