#include "MenuLayer.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Events/MenuEvent.h"

MenuLayer::MenuLayer()
	: Layer("Menu"), m_Menu(false), m_Controls(false), m_GameState(GameState::Running)
{ }

void MenuLayer::OnAttach()
{
	float width = Engine::Application::Get().GetWindow().GetWidth();
	float height = Engine::Application::Get().GetWindow().GetHeight();
	m_MenuCamera.Projection = glm::ortho(-(width / 2.0f), width / 2.0f, -(height / 2.0f), height / 2.0f, 10.0f, -10.0f);
	m_MenuCamera.Transform = glm::mat4(1);

	Engine::OpenGL::Texture2DSpecification spec;
	spec.Min_Filter = 0x2600;
	spec.Mag_Filter = 0x2600;

	m_ControlsTex = Engine::CreateRef<Engine::OpenGL::GlTexture2D>("Controls", spec, "assets/textures/Controls.png");
	m_Victory = Engine::CreateRef<Engine::OpenGL::GlTexture2D>("Victory", "assets/textures/Victory.png");
	m_Defeat = Engine::CreateRef<Engine::OpenGL::GlTexture2D>("Defeat", "assets/textures/Defeat.png");
}

void MenuLayer::OnDetach()
{
}

void MenuLayer::OnUpdate(Engine::Timestep ts)
{
	static glm::mat4 menuTransform = glm::scale(glm::mat4(1), {1238, 700, 1});
	static glm::mat4 endTransform = glm::translate(glm::mat4(1), { 0, 0, 1 }) * glm::scale(glm::mat4(1), { 1400 * 0.5, 252 * 0.5, 1 });

	Engine::Renderer2D::BeginScene(m_MenuCamera);

	if (m_Controls)
	{
		Engine::Renderer2D::DrawQuad(menuTransform, m_ControlsTex);
	}

	if (m_GameState != GameState::Running)
	{
		Engine::Renderer2D::DrawQuad(endTransform, m_GameState == GameState::Victory ? m_Victory : m_Defeat);
	}

	Engine::Renderer2D::EndScene();
}

void MenuLayer::OnEvent(Engine::Event& event)
{
	Engine::EventHandler eventHandler(event);
	eventHandler.Handle<Engine::KeyPressedEvent>(EG_BIND_EVENT_FN(MenuLayer::OnKeyPressed));
	eventHandler.Handle<Engine::WindowResizeEvent>(EG_BIND_EVENT_FN(MenuLayer::OnWindowResize));
	eventHandler.HandleGameEvent<GameEndEvent>(EG_BIND_EVENT_FN(MenuLayer::OnGameEnd));
}

bool MenuLayer::OnKeyPressed(Engine::KeyPressedEvent& event)
{
	if (event.GetKeyCode() == Engine::Key::Escape)
	{
		if (m_Menu)
			HideMenu();
		else
			ShowMenu();
	}

	if (event.GetKeyCode() == Engine::Key::F1)
	{
		m_Controls = !m_Controls;
	}

	if (event.GetKeyCode() == Engine::Key::F11)
	{
		Engine::Application::Get().GetWindow().ToggleFullscreen();
	}

	if (event.GetKeyCode() == Engine::Key::Comma || event.GetKeyCode() == Engine::Key::KPSubtract)
	{
		float exposure = Engine::Renderer::GetExposure();
		Engine::Renderer::SetExposure(exposure - 0.1f);
	}

	if (event.GetKeyCode() == Engine::Key::Period || event.GetKeyCode() == Engine::Key::KPAdd)
	{
		float exposure = Engine::Renderer::GetExposure();
		Engine::Renderer::SetExposure(exposure + 0.1f);
	}

	return false;
}

bool MenuLayer::OnWindowResize(Engine::WindowResizeEvent& event)
{
	m_MenuCamera.Projection = glm::ortho(-(event.GetWidth() / 2.0f), event.GetWidth() / 2.0f, -(event.GetHeight() / 2.0f), event.GetHeight() / 2.0f, 10.0f, -10.0f);
	return false;
}

bool MenuLayer::OnGameEnd(GameEndEvent& event)
{
	m_GameState = event.Victory() ? GameState::Victory : GameState::Defeat;
	Engine::Application::Get().PopLayer();
	return false;
}

void MenuLayer::ShowMenu()
{
	m_Menu = true;
	Engine::Application::Get().OnEvent(MenuEvent(m_Menu));

	float x = Engine::Application::Get().GetWindow().GetWidth() / 2;
	float y = Engine::Application::Get().GetWindow().GetHeight() / 2;
	Engine::Application::Get().GetWindow().ShowCursor();
	Engine::Application::Get().GetWindow().SetCursorPosition(x, y);

}

void MenuLayer::HideMenu()
{
	Engine::Application::Get().GetWindow().HideCursor();

	m_Menu = false;
	Engine::Application::Get().OnEvent(MenuEvent(m_Menu));
}

