#include "Hero.h"
#include "Engine/Physics/PhysicsAPI.h"
#include "Components/GameComponents.h"
#include "Engine/Scene/Factories.h"
#include "Events/CharacterHealthEvent.h"
#include "Events/CharacterManaEvent.h"

using TransformComponent			= Engine::Component::Core::TransformComponent;
using NativeScriptComponent			= Engine::Component::Core::NativeScriptComponent;
using ParentComponent				= Engine::Component::Core::ParentComponent;

using RigidDynamicComponent			= Engine::Component::Physics::RigidDynamicComponent;
using ShapeComponent				= Engine::Component::Physics::ShapeComponent;
using TriggerComponent				= Engine::Component::Physics::TriggerComponent;
using KinematicComponent			= Engine::Component::Physics::KinematicComponent;
using KinematicMovementComponent	= Engine::Component::Physics::KinematicMovementComponent;

using MaterialComponent				= Engine::Component::Renderer::MaterialComponent;
using ShadowComponent				= Engine::Component::Renderer::ShadowComponent;
using PointLightComponent			= Engine::Component::Renderer::PointLightComponent;
using MeshComponent					= Engine::Component::Renderer::MeshComponent;

void Hero::OnEvent(Engine::Event& e)
{
	Engine::EventHandler eventHandler(e);
	eventHandler.Handle<Engine::MouseButtonPressedEvent>(EG_BIND_EVENT_FN(Hero::OnMouseButtonPressed));
	eventHandler.Handle<Engine::MouseScrolledEvent>(EG_BIND_EVENT_FN(Hero::OnMouseScrolled));
	eventHandler.Handle<Engine::KeyPressedEvent>(EG_BIND_EVENT_FN(Hero::OnKeyPressed));
}

bool Hero::OnMouseButtonPressed(Engine::MouseButtonPressedEvent& e)
{
	if (e.GetMouseButton() == Engine::Mouse::ButtonLeft)
		UseRightHand();

	if (e.GetMouseButton() == Engine::Mouse::ButtonRight)
		UseLeftHand();

	return false;
}

bool Hero::OnMouseScrolled(Engine::MouseScrolledEvent& e)
{
	switch (m_ActiveSpell)
	{
	case MagicBallType::Fire:
		m_ActiveSpell = MagicBallType::Lightning;
		break;
	default:
		m_ActiveSpell = MagicBallType::Fire;
	}

	if (m_RightHand)
		DropRight();

	m_RightHand = CreateMagicBall(m_ActiveSpell, { 0.42f, 0.68f, -1.0f });

	return false;
}

bool Hero::OnKeyPressed(Engine::KeyPressedEvent& e)
{
	if (e.GetKeyCode() == Engine::Key::E || e.GetKeyCode() == Engine::Key::Q)
	{
		switch (m_PassiveSpell)
		{
		case MagicBallType::Light:
			m_PassiveSpell = MagicBallType::Heal;
			break;
		default:
			m_PassiveSpell = MagicBallType::Light;
		}

		if (m_LeftHand)
			DropLeft();

		m_LeftHand = CreateMagicBall(m_PassiveSpell, { -0.5f, 0.65f, -1.0f });
	}
	
	return false;
}

void Hero::UseLeftHand()
{
	if (m_LeftHand)
	{
		auto& heroComp = GetComponent<HeroComponent>();
		
		switch (m_PassiveSpell)
		{
		case MagicBallType::Light:
			Light(m_LeftHand);
			break;
		case MagicBallType::Heal:
			if (heroComp.Mana >= 10.0f)
				Heal(m_LeftHand);
			break;
		default:
			m_PassiveSpell = MagicBallType::Light;
		}
	}
	else
		m_LeftHand = CreateMagicBall(m_PassiveSpell, { -0.5f, 0.65f, -1.0f });
}

void Hero::UseRightHand()
{
	if (m_RightHand)
	{
		auto& heroComp = GetComponent<HeroComponent>();
		if(heroComp.Mana >= 10.0f)
		{
			Throw(m_RightHand);
			m_RightHand = Engine::Entity();
		}
	}
	else
		m_RightHand = CreateMagicBall(m_ActiveSpell, { 0.42f, 0.68f, -1.0f });
}

Engine::Entity Hero::CreateMagicBall(MagicBallType type, glm::vec3 offset)
{
	Engine::Entity ball{ Engine::Factory::CreateEntity(*m_RegistryHandle, "MagicBall"), m_RegistryHandle };
	auto& transformComp = ball.GetComponent<TransformComponent>();

	transformComp.Scale = { 0.1f, 0.1f, 0.1f };
	transformComp.Translation = offset;

	ball.AddComponent<ParentComponent>(m_EntityHandle);
	ball.AddComponent<MeshComponent>(Engine::MeshLibrary::Get("Sphere"));

	switch (type)
	{
	case MagicBallType::Light:
		ball.AddComponent<MaterialComponent>(Engine::MaterialLibrary::Get("MagicBall_Light"));
		ball.AddComponent<MagicBallComponent>(nullptr, Engine::SoundLibrary::Get("LightCast"), nullptr);
		break;
	case MagicBallType::Heal:
		ball.AddComponent<MaterialComponent>(Engine::MaterialLibrary::Get("MagicBall_Heal"));
		ball.AddComponent<MagicBallComponent>(nullptr, nullptr, nullptr);
		break;
	case MagicBallType::Fire:
		ball.AddComponent<ShadowComponent>();
		ball.AddComponent<MaterialComponent>(Engine::MaterialLibrary::Get("MagicBall_Fire"));
		ball.AddNativeScript<MagicBall>();
		ball.AddComponent<MagicBallComponent>(nullptr, Engine::SoundLibrary::Get("FireballShoot"), Engine::SoundLibrary::Get("Impact"));
		ball.GetComponent<NativeScriptComponent>().Active = false;
		break;
	case MagicBallType::Lightning:
		ball.AddComponent<ShadowComponent>();
		ball.AddComponent<MaterialComponent>(Engine::MaterialLibrary::Get("MagicBall_Water"));
		ball.AddNativeScript<MagicBall>();
		ball.AddComponent<MagicBallComponent>(Engine::SoundLibrary::Get("LightningShoot"), nullptr, Engine::SoundLibrary::Get("Thunder"));
		ball.GetComponent<NativeScriptComponent>().Active = false;
		break;
	}

	auto& magicBallComp = ball.GetComponent<MagicBallComponent>();
	if(magicBallComp.CastSound != nullptr)
		Engine::SoundEngine::Get()->play2D(magicBallComp.CastSound);

	return ball;
}

void Hero::DropLeft()
{
	m_LeftHand.Destroy();
	m_LeftHand = Engine::Entity();
}

void Hero::DropRight()
{
	m_RightHand.Destroy();
	m_RightHand = Engine::Entity();
}

void Hero::Light(Engine::Entity ball)
{
	if (ball.HasComponent<PointLightComponent>())
		ball.RemoveComponent<PointLightComponent>();
	else
	{
		ball.AddComponent<PointLightComponent>(glm::vec3{ 0.85f, 0.7f, 0.7f }, 0.4f, 0.1f, 0.05f);
	}
}

void Hero::Heal(Engine::Entity ball)
{
	if (ball.HasComponent<PointLightComponent>())
		ball.RemoveComponent<PointLightComponent>();
	else
	{
		auto& heroComp = GetComponent<HeroComponent>();
		

		if (heroComp.Hitpoints < 100.0f)
		{
			ball.AddComponent<PointLightComponent>(glm::vec3{ 0.85f, 0.7f, 0.0f }, 0.4f, 0.1f, 0.05f);

			if (heroComp.Hitpoints < 80.0f)
				heroComp.Hitpoints += 20.0f;
			else
				heroComp.Hitpoints = 100.0f;

			if (heroComp.Mana >= 10.0f)
				heroComp.Mana -= 10.0f;

			CharacterHealthEvent healthEvent(heroComp.Hitpoints);
			CharacterManaEvent manaEvent(heroComp.Mana);
			Engine::Application::Get().OnEvent(healthEvent);
			Engine::Application::Get().OnEvent(manaEvent);
		}
	}
}

void Hero::Throw(Engine::Entity ball)
{
	Engine::System::Util::MakeIndependent(*m_RegistryHandle, ball);

	auto& transformComp = GetComponent<TransformComponent>();
	auto& transformComp_ball = ball.GetComponent<TransformComponent>();
	auto& magicBallComp = ball.GetComponent<MagicBallComponent>();

	auto& heroComp = GetComponent<HeroComponent>();
	
	if (heroComp.Mana >= 10.0f)
		heroComp.Mana -= 10.0f;
	
	CharacterManaEvent event(heroComp.Mana);
	Engine::Application::Get().OnEvent(event);
	
	if(magicBallComp.ThrowSound != nullptr)
		Engine::SoundEngine::Get()->play2D(magicBallComp.ThrowSound);
	
	glm::vec4 velocity = glm::toMat4(glm::quat(transformComp.Rotation)) * glm::vec4{ 0.0f, 0.0f, -28.0f, 0.0 };
	
	auto* actor = Engine::PhysicsAPI::CreateRigidDynamic(transformComp_ball.Translation);
	auto* shape = Engine::PhysicsAPI::CreateSphereShape(transformComp_ball.Scale.x);
	shape->setContactOffset(0.1f);

	ball.AddComponent<RigidDynamicComponent>(actor);
	ball.AddComponent<ShapeComponent>(shape);

	ball.AddComponent<TriggerComponent>();
	ball.AddComponent<KinematicComponent>();
	ball.AddComponent<KinematicMovementComponent>(glm::vec3{ velocity.x, velocity.y, velocity.z });
	
	ball.GetComponent<NativeScriptComponent>().Active = true;
}
