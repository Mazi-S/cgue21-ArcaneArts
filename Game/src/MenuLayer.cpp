#include "MenuLayer.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

MenuLayer::MenuLayer()
	: Layer("Menu"), m_Menu(false), m_GameState(GameState::Running)
{ }

void MenuLayer::OnAttach()
{
	float width = Engine::Application::Get().GetWindow().GetWidth();
	float height = Engine::Application::Get().GetWindow().GetHeight();
	m_Camera.Projection = glm::ortho(-(width / 2.0f), width / 2.0f, -(height / 2.0f), height / 2.0f, 10.0f, -10.0f);
	m_Camera.Transform = glm::mat4(1);

	Engine::OpenGL::Texture2DSpecification spec;
	spec.Min_Filter = 0x2600;
	spec.Mag_Filter = 0x2600;

	m_Controls = Engine::CreateRef<Engine::OpenGL::GlTexture2D>("Controls", spec, "assets/textures/Controls.png");
	m_Victory = Engine::CreateRef<Engine::OpenGL::GlTexture2D>("Victory", "assets/textures/Victory.png");
	m_Defeat = Engine::CreateRef<Engine::OpenGL::GlTexture2D>("Defeat", "assets/textures/Defeat.png");
}

void MenuLayer::OnDetach()
{
}

void MenuLayer::OnUpdate(Engine::Timestep ts)
{
	static glm::mat4 menuTransform = glm::scale(glm::mat4(1), {1305, 675, 1});
	static glm::mat4 endTransform = glm::translate(glm::mat4(1), { 0, 0, 1 }) * glm::scale(glm::mat4(1), { 1400 * 0.5, 252 * 0.5, 1 });

	Engine::Renderer2D::BeginScene(m_Camera);

	if (m_Menu)
	{
		Engine::Renderer2D::DrawQuad(menuTransform, m_Controls);
	}

	if (m_GameState != GameState::Running)
	{
		Engine::Renderer2D::DrawQuad(endTransform, m_GameState == GameState::Victory ? m_Victory : m_Defeat);
	}

	Engine::Renderer2D::EndScene();
}

void MenuLayer::OnEvent(Engine::Event& event)
{
	if (m_Menu && event.IsInCategory(Engine::EventCategory::Input))
		event.Handled = true;

	Engine::EventHandler eventHandler(event);
	eventHandler.Handle<Engine::KeyPressedEvent>(EG_BIND_EVENT_FN(MenuLayer::OnKeyPressed));
	eventHandler.Handle<Engine::WindowResizeEvent>(EG_BIND_EVENT_FN(MenuLayer::OnWindowResize));
	eventHandler.HandleGameEvent<GameEndEvent>(EG_BIND_EVENT_FN(MenuLayer::OnGameEnd));
}

bool MenuLayer::OnKeyPressed(Engine::KeyPressedEvent& event)
{
	if (event.GetKeyCode() == Engine::Key::Escape)
	{
		m_Menu = !m_Menu;

		float x = Engine::Application::Get().GetWindow().GetWidth() / 2;
		float y = Engine::Application::Get().GetWindow().GetHeight() / 2;
		Engine::Application::Get().GetWindow().SetCursorPosition(x, y);
		Engine::Application::Get().GetWindow().ShowCursor();

		return false;
	}

	if (event.GetKeyCode() == Engine::Key::F11)
	{
		Engine::Application::Get().GetWindow().ToggleFullscreen();
		return false;
	}

	return m_Menu;
}

bool MenuLayer::OnWindowResize(Engine::WindowResizeEvent& event)
{
	m_Camera.Projection = glm::ortho(-(event.GetWidth() / 2.0f), event.GetWidth() / 2.0f, -(event.GetHeight() / 2.0f), event.GetHeight() / 2.0f, 10.0f, -10.0f);
	return false;
}

bool MenuLayer::OnGameEnd(GameEndEvent& event)
{
	m_GameState = event.Victory() ? GameState::Victory : GameState::Defeat;
	return false;
}
