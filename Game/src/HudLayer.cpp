#include "HudLayer.h"

#include <Engine/Util/ObjectLoader.h>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Engine/Core/Base.h"
#include "Engine/Scene/Systems.h"
#include "Engine/Scene/Factories.h"

#include <algorithm>

#include "Events/CharacterManaEvent.h"
#include "GameApp.h";

HudLayer::HudLayer()
	: Layer("HUD"), m_HUD(true)
{
}

void HudLayer::OnAttach()
{
	float width = Engine::Application::Get().GetWindow().GetWidth();
	float height = Engine::Application::Get().GetWindow().GetHeight();
	m_Camera.Projection = glm::ortho(-(width / 2.0f), width / 2.0f, -(height / 2.0f), height / 2.0f, 10.0f, -10.0f );
	m_Camera.Transform = glm::mat4(1);

	auto crosshairTex = Engine::CreateRef<Engine::OpenGL::GlTexture2D>("Crosshair", "assets/textures/crosshair.png");

	Engine::Entity crosshair{ Engine::Factory::CreateEntity(m_Registry, "Crosshair"), &m_Registry};
	crosshair.GetComponent<Engine::Component::Core::TransformComponent>().Scale = { 20, 20, 1 };
	crosshair.AddComponent<Engine::Component::Renderer2D::SpriteRendererComponent>(glm::vec4(1, 1, 1, 0.4));
	crosshair.AddComponent<Engine::Component::Renderer2D::SpriteRendererTextureComponent>(crosshairTex);

	// HealthBar
	m_HealthBar.Init(m_Registry);

	// ManaBar
	m_ManaBar.Init(m_Registry);

	// Monster Display
	Engine::OpenGL::Texture2DSpecification spec;
	spec.Min_Filter = 0x2600;
	spec.Mag_Filter = 0x2600;
	auto bigIcon = Engine::CreateRef<Engine::OpenGL::GlTexture2D>("monsterIcon", spec, "assets/textures/monster-icon.png");
	m_MonsterDisplay.Init(m_Registry, bigIcon, 6);
}

void HudLayer::OnDetach()
{
}

void HudLayer::OnUpdate(Engine::Timestep ts)
{
	if (m_HUD)
	{
		Engine::Renderer2D::BeginScene(m_Camera);

		Engine::System::Renderer2D::SubmitSprites(m_Registry);

		Engine::Renderer2D::EndScene();
	}
}

void HudLayer::OnEvent(Engine::Event& event)
{
	Engine::EventHandler eventHandler(event);
	eventHandler.Handle<Engine::WindowResizeEvent>(EG_BIND_EVENT_FN(HudLayer::OnWindowResize));
	eventHandler.Handle<Engine::KeyPressedEvent>(EG_BIND_EVENT_FN(HudLayer::OnKeyPressed));
	eventHandler.HandleGameEvent<CharacterHealthEvent>(EG_BIND_EVENT_FN(HudLayer::OnHealthChange));
	eventHandler.HandleGameEvent<MonsterDiedEvent>(EG_BIND_EVENT_FN(HudLayer::OnMonsterDied));
	eventHandler.HandleGameEvent<CharacterManaEvent>(EG_BIND_EVENT_FN(HudLayer::OnManaChange));
	eventHandler.HandleGameEvent<GameEndEvent>(EG_BIND_EVENT_FN(HudLayer::OnGameEnd));
}

bool HudLayer::OnWindowResize(Engine::WindowResizeEvent& event)
{
	m_Camera.Projection = glm::ortho(-(event.GetWidth() / 2.0f), event.GetWidth() / 2.0f, -(event.GetHeight() / 2.0f), event.GetHeight() / 2.0f, 10.0f, -10.0f);

	glm::vec2 posHealth = { -(event.GetWidth() / 2.0f) + m_HealthBar.Width / 2.0f + m_HealthBar.Offset, event.GetHeight() / 2.0f - m_HealthBar.Height / 2.0f - m_HealthBar.Offset };
	m_HealthBar.UpdatePosition(posHealth);

	glm::vec2 posMana = { -(event.GetWidth() / 2.0f) + m_ManaBar.Width / 2.0f + m_ManaBar.OffsetTop, event.GetHeight() / 2.0f - m_ManaBar.Height / 2.0f - m_ManaBar.OffsetLeft };
	m_ManaBar.UpdatePosition(posMana);

	glm::vec2 rightTop = { event.GetWidth() / 2.0f, event.GetHeight() / 2.0 };
	m_MonsterDisplay.UpdatePosition(rightTop);
	
	return false;
}

bool HudLayer::OnHealthChange(CharacterHealthEvent& event)
{
	m_HealthBar.UpdateHealth(event.GetHealth());
	return false;
}

bool HudLayer::OnMonsterDied(MonsterDiedEvent& event)
{
	m_MonsterDisplay.Remove();
	return true;
}

bool HudLayer::OnManaChange(CharacterManaEvent& event)
{
	m_ManaBar.UpdateMana(event.GetMana());
	return false;
}

bool HudLayer::OnGameEnd(GameEndEvent& event)
{
	Engine::Application::Get().Remove(this);
	return false;
}

bool HudLayer::OnKeyPressed(Engine::KeyPressedEvent& event)
{
	if (event.GetKeyCode() == Engine::Key::F2)
		m_HUD = !m_HUD;

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

void ManaBar::Init(entt::registry& registry)
{
	Bar = { Engine::Factory::CreateEntity(registry, "ManaBar"), &registry };
	float windowWidth = Engine::Application::Get().GetWindow().GetWidth();
	float windowHeight = Engine::Application::Get().GetWindow().GetHeight();
	glm::vec2 pos = { -(windowWidth / 2.0f) + Width / 2.0f + OffsetTop, windowHeight / 2.0f - Height / 2.0f - OffsetLeft };
	UpdatePosition(pos);

	Background = { Engine::Factory::CreateEntity(registry, "ManaBar_background"), &registry };
	Background.GetComponent<Engine::Component::Core::TransformComponent>().Scale = { Width, Height, 1 };
	Background.GetComponent<Engine::Component::Core::TransformComponent>().Translation = { 0, 0, 0 };
	Background.AddComponent<Engine::Component::Renderer2D::SpriteRendererComponent>(glm::vec4(.6, .6, .6, 0.6));
	Background.AddComponent<Engine::Component::Core::ParentComponent>(Bar);

	Mana = { Engine::Factory::CreateEntity(registry, "ManaBar_mana"), &registry };
	Mana.AddComponent<Engine::Component::Renderer2D::SpriteRendererComponent>(glm::vec4(0, 0, .8, 0.9));
	Mana.AddComponent<Engine::Component::Core::ParentComponent>(Bar);
	UpdateMana(100);
}

void HealthBar::UpdateHealth(float hp)
{
	float health = std::max(hp, 0.0f);
	float healthWidth = Width * (health / 100.0f);

	auto& transformComp = Health.GetComponent<Engine::Component::Core::TransformComponent>();
	transformComp.Scale = { healthWidth, Height, 1 };
	transformComp.Translation = { -(Width - healthWidth) / 2.0f, 0, -0.1f };

	if(health <= 0)
		GameApp::Defeat();
}

void ManaBar::UpdateMana(float m)
{
	float mana = std::max(m, 0.0f);
	float manaWidth = Width * (mana / 100.0f);

	auto& transformComp = Mana.GetComponent<Engine::Component::Core::TransformComponent>();
	transformComp.Scale = { manaWidth, Height, 1 };
	transformComp.Translation = { -(Width - manaWidth) / 2.0f, 0, -0.1f };
}

void HealthBar::UpdatePosition(glm::vec2 position)
{
	auto& transformComp = Bar.GetComponent<Engine::Component::Core::TransformComponent>();
	transformComp.Translation = { position.x, position.y, 0.0f };
}

void ManaBar::UpdatePosition(glm::vec2 position)
{
	auto& transformComp = Bar.GetComponent<Engine::Component::Core::TransformComponent>();
	transformComp.Translation = { position.x, position.y, 0.0f };
}

void MonsterDisplay::Init(entt::registry& registry, Engine::Ref<Engine::OpenGL::GlTexture2D> sprite, uint32_t count)
{
	Sprite = sprite;
	Registry = &registry;

	Bar = { Engine::Factory::CreateEntity(registry, "MonsterDisplay"), &registry };
	float windowWidth = Engine::Application::Get().GetWindow().GetWidth();
	float windowHeight = Engine::Application::Get().GetWindow().GetHeight();
	glm::vec2 pos = { windowWidth / 2.0, windowHeight / 2.0 };
	UpdatePosition(pos);

	for (uint32_t i = 0; i < count; i++)
		Add();
}

void MonsterDisplay::Add()
{
	Engine::Entity monster = { Engine::Factory::CreateEntity(*Registry, "MonsterIcon"), Registry };

	monster.GetComponent<Engine::Component::Core::TransformComponent>().Scale = { Size.x, Size.y, 1 };
	monster.GetComponent<Engine::Component::Core::TransformComponent>().Translation = { - ((OffsetMonster + Size.x) * Monsters.size()), 0, 0 };
	monster.AddComponent<Engine::Component::Renderer2D::SpriteRendererComponent>(glm::vec4(1, 1, 1, 0.9));
	monster.AddComponent<Engine::Component::Renderer2D::SpriteRendererTextureComponent>(Sprite);
	monster.AddComponent<Engine::Component::Core::ParentComponent>(Bar);
	
	Monsters.push_back(monster);
}

void MonsterDisplay::Remove()
{
	if (!Monsters.empty())
	{
		Monsters.back().Destroy();
		Monsters.pop_back();

		if (Monsters.size() == 0)
			GameApp::Victory();
	}
}

void MonsterDisplay::UpdatePosition(glm::vec2 position)
{
	auto& transformComp = Bar.GetComponent<Engine::Component::Core::TransformComponent>();
	transformComp.Translation = { position.x - OffsetRight - Size.x / 2.0, position.y - OffsetTop - Size.y / 2.0, 0.0f };
}
