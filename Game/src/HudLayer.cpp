#include "HudLayer.h"

#include <Engine/Events/KeyEvent.h>
#include <Engine/Util/ObjectLoader.h>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Engine/Core/Base.h"

HudLayer::HudLayer()
	: Layer("HUD")
{
}

void HudLayer::OnAttach()
{

	float width = Engine::Application::Get().GetWindow().GetWidth();
	float height = Engine::Application::Get().GetWindow().GetHeight();
	m_Camera.Projection = glm::ortho(-(width / 2.0f), width / 2.0f, -(height / 2.0f), height / 2.0f );
	m_Camera.Transform = glm::mat4(1);

	Engine::TextureLibrary::LoadTexture2D("Crosshair", "assets/textures/crosshair.png");
}

void HudLayer::OnDetach()
{
}

void HudLayer::OnUpdate(Engine::Timestep ts)
{
	Engine::Renderer2D::BeginScene(m_Camera);
	Engine::Renderer2D::DrawQuad(glm::scale(glm::mat4(1), { 20, 20, 20 }), Engine::TextureLibrary::GetTexture2D("Crosshair"), { 1, 1, 1, .3 });
	Engine::Renderer2D::EndScene();
}

void HudLayer::OnEvent(Engine::Event& event)
{
	Engine::EventHandler eventHandler(event);
	eventHandler.Handle<Engine::WindowResizeEvent>(EG_BIND_EVENT_FN(HudLayer::OnWindowResize));
}

bool HudLayer::OnWindowResize(Engine::WindowResizeEvent& event)
{
	m_Camera.Projection = glm::ortho(-(event.GetWidth() / 2.0f), event.GetWidth() / 2.0f, -(event.GetHeight() / 2.0f), event.GetHeight() / 2.0f);
	return false;
}
