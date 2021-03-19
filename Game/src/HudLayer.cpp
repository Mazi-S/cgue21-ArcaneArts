#include "HudLayer.h"

#include <Engine/Events/KeyEvent.h>
#include <Engine/Util/ObjectLoader.h>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Engine/Core/Base.h"
#include "Engine/Scene/Systems.h"
#include "Engine/Scene/Factories.h"

HudLayer::HudLayer()
	: Layer("HUD")
{
}

void HudLayer::OnAttach()
{
	this->SetHitpoints(100);

	float width = Engine::Application::Get().GetWindow().GetWidth();
	float height = Engine::Application::Get().GetWindow().GetHeight();
	m_Camera.Projection = glm::ortho(-(width / 2.0f), width / 2.0f, -(height / 2.0f), height / 2.0f, 10.0f, -10.0f );
	m_Camera.Transform = glm::mat4(1);

	Engine::TextureLibrary::LoadTexture2D("Crosshair", "assets/textures/crosshair.png");

	Engine::Entity crosshair{ Engine::Factory::CreateEntity(m_Registry, "Crosshair"), &m_Registry};
	crosshair.GetComponent<Engine::Component::Core::TransformComponent>().Scale = { 20,20,20 };
	crosshair.AddComponent<Engine::Component::Renderer2D::SpriteRendererComponent>(glm::vec4(1, 1, 1, 0.4));
	crosshair.AddComponent<Engine::Component::Renderer2D::SpriteRendererTextureComponent>(Engine::TextureLibrary::GetTexture2D("Crosshair"));

	Engine::Entity temp{ Engine::Factory::CreateEntity(m_Registry, "temp"), &m_Registry };
	temp.GetComponent<Engine::Component::Core::TransformComponent>().Scale = { 5*m_Hitpoints, 20, 1 };
	temp.GetComponent<Engine::Component::Core::TransformComponent>().Translation = { -500, 500, 0 };
	temp.AddComponent<Engine::Component::Renderer2D::SpriteRendererComponent>(glm::vec4(0, 1, 1, 0.6));
}

void HudLayer::OnDetach()
{
}

void HudLayer::OnUpdate(Engine::Timestep ts)
{
	Engine::Renderer2D::BeginScene(m_Camera);

	Engine::System::Renderer2D::SubmitSprites(m_Registry);

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
