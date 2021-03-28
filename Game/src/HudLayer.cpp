#include "HudLayer.h"

#include <Engine/Util/ObjectLoader.h>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Engine/Core/Base.h"
#include "Engine/Scene/Systems.h"
#include "Engine/Scene/Factories.h"

#include <algorithm>

HudLayer::HudLayer()
	: Layer("HUD")
{
}

void HudLayer::OnAttach()
{
	float width = Engine::Application::Get().GetWindow().GetWidth();
	float height = Engine::Application::Get().GetWindow().GetHeight();
	m_Camera.Projection = glm::ortho(-(width / 2.0f), width / 2.0f, -(height / 2.0f), height / 2.0f, 10.0f, -10.0f );
	m_Camera.Transform = glm::mat4(1);

	Engine::TextureLibrary::LoadTexture2D("Crosshair", "assets/textures/crosshair.png");

	Engine::Entity crosshair{ Engine::Factory::CreateEntity(m_Registry, "Crosshair"), &m_Registry};
	crosshair.GetComponent<Engine::Component::Core::TransformComponent>().Scale = { 20,20,20 };
	crosshair.AddComponent<Engine::Component::Renderer2D::SpriteRendererComponent>(glm::vec4(1, 1, 1, 0.4));
	crosshair.AddComponent<Engine::Component::Renderer2D::SpriteRendererTextureComponent>(Engine::TextureLibrary::GetTexture2D("Crosshair"));

	// HealthBar
	m_HealthBar.Init(m_Registry);
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
	eventHandler.HandleGameEvent<CharacterHealthEvent>(EG_BIND_EVENT_FN(HudLayer::OnHealthChange));
}

bool HudLayer::OnWindowResize(Engine::WindowResizeEvent& event)
{
	m_Camera.Projection = glm::ortho(-(event.GetWidth() / 2.0f), event.GetWidth() / 2.0f, -(event.GetHeight() / 2.0f), event.GetHeight() / 2.0f, 10.0f, -10.0f);

	glm::vec2 pos = { -(event.GetWidth() / 2.0f) + m_HealthBar.Width / 2.0f + m_HealthBar.Offset, event.GetHeight() / 2.0f - m_HealthBar.Height / 2.0f - m_HealthBar.Offset };
	m_HealthBar.UpdatePosition(pos);
	return false;
}

bool HudLayer::OnHealthChange(CharacterHealthEvent& event)
{
	m_HealthBar.UpdateHealth(event.GetHealth());
	return false;
}

void HealthBar::Init(entt::registry& registry)
{
	Bar = { Engine::Factory::CreateEntity(registry, "HealthBar"), &registry };
	float windowWidth = Engine::Application::Get().GetWindow().GetWidth();
	float windowHeight = Engine::Application::Get().GetWindow().GetHeight();
	glm::vec2 pos = { -(windowWidth / 2.0f) + Width / 2.0f + Offset, windowHeight / 2.0f - Height / 2.0f - Offset };
	UpdatePosition(pos);

	Background = { Engine::Factory::CreateEntity(registry, "HealthBar_background"), &registry };
	Background.GetComponent<Engine::Component::Core::TransformComponent>().Scale = { Width, Height, 1 };
	Background.GetComponent<Engine::Component::Core::TransformComponent>().Translation = { 0, 0, 0 };
	Background.AddComponent<Engine::Component::Renderer2D::SpriteRendererComponent>(glm::vec4(.6, .6, .6, 0.6));
	Background.AddComponent<Engine::Component::Core::ParentComponent>(Bar);

	Health = { Engine::Factory::CreateEntity(registry, "HealthBar_health"), &registry };
	Health.AddComponent<Engine::Component::Renderer2D::SpriteRendererComponent>(glm::vec4(.8, 0, 0, 0.9));
	Health.AddComponent<Engine::Component::Core::ParentComponent>(Bar);
	UpdateHealth(100);
}

void HealthBar::UpdateHealth(float hp)
{
	float health = std::max(hp, 0.0f);
	float healthWidth = Width * (health / 100.0f);

	auto& transformComp = Health.GetComponent<Engine::Component::Core::TransformComponent>();
	transformComp.Scale = { healthWidth, Height, 1 };
	transformComp.Translation = { -(Width - healthWidth) / 2.0f, 0, -0.1f };
}

void HealthBar::UpdatePosition(glm::vec2 position)
{
	auto& transformComp = Bar.GetComponent<Engine::Component::Core::TransformComponent>();
	transformComp.Translation = { position.x, position.y, 0.0f };
}
