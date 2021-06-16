#include "HeroScript.h"
#include "Engine/Scene/Factories.h"

#include "Components/GameComponents.h"
#include "ActorFactory.h"

#include "Events/CharacterHealthEvent.h"
#include "Events/CharacterManaEvent.h"
#include <random>

using TransformComponent			= Engine::Component::Core::TransformComponent;
using NativeScriptComponent			= Engine::Component::Core::NativeScriptComponent;
using ParentComponent				= Engine::Component::Core::ParentComponent;
using Unserializable				= Engine::Component::Core::Unserializable;

using PhysicsMaterialComponent		= Engine::Component::Physics::PhysicsMaterialComponent;
using DynamicConvexComponent		= Engine::Component::Physics::DynamicConvexComponent;
using RigidDynamicComponent			= Engine::Component::Physics::RigidDynamicComponent;
using KinematicMovementComponent	= Engine::Component::Physics::KinematicMovementComponent;

using Sound2DComponent				= Engine::Component::Audio::Sound2DComponent;

using MaterialComponent				= Engine::Component::Renderer::MaterialComponent;
using ShadowComponent				= Engine::Component::Renderer::ShadowComponent;
using PointLightComponent			= Engine::Component::Renderer::PointLightComponent;
using MeshComponent					= Engine::Component::Renderer::MeshComponent;
using ParticleSystemComponent		= Engine::Component::Renderer::ParticleSystemComponent;

static glm::vec3 s_ActiveHandOffset = { 0.77f, -0.35f, -1.58f };
static glm::vec3 s_PassiveHandOffset = { -0.67f, -0.3f, -1.58f };

static glm::vec3 s_MagicBallScale = { 0.1f, 0.1f, 0.1f };

HeroScript::HeroScript(Engine::Entity entity) : ScriptableEntity(entity)
{
	EquipActive(MagicBallType::Fire);
}

void HeroScript::OnUpdate(Engine::Timestep ts)
{
	bool casting = false;

	if (Engine::Input::IsMouseButtonPressed(Engine::Mouse::ButtonLeft))
	{
		casting = UseActive(ts);
	}

	if (Engine::Input::IsMouseButtonPressed(Engine::Mouse::ButtonRight))
	{
		casting = UsePassive(ts);
	}

	UpdateMana(ts, !casting);
	UpdateHealth();

}

void HeroScript::OnEvent(Engine::Event& e)
{
	Engine::EventHandler eventHandler(e);
	eventHandler.Handle<Engine::MouseButtonPressedEvent>(EG_BIND_EVENT_FN(HeroScript::OnMouseButtonPressed));
	eventHandler.Handle<Engine::MouseButtonReleasedEvent>(EG_BIND_EVENT_FN(HeroScript::OnMouseButtonReleased));
	eventHandler.Handle<Engine::MouseScrolledEvent>(EG_BIND_EVENT_FN(HeroScript::OnMouseScrolled));
	eventHandler.Handle<Engine::KeyPressedEvent>(EG_BIND_EVENT_FN(HeroScript::OnKeyPressed));
}

bool HeroScript::OnMouseButtonPressed(Engine::MouseButtonPressedEvent& e)
{
	if (e.GetMouseButton() == Engine::Mouse::ButtonLeft)
	{
		bool created = StartActive();
		//TODO: play sound if failed!!!
	}

	if (e.GetMouseButton() == Engine::Mouse::ButtonRight)
	{
		if (m_PassiveHand && m_PassiveHand.HasComponent<MagicBallComponent>())
		{
			auto& magicBallComp = m_PassiveHand.GetComponent<MagicBallComponent>();
			if (!magicBallComp.ThrowSound.empty())
			{
				auto& sound2DComp = m_PassiveHand.EmplaceOrReplace<Sound2DComponent>(magicBallComp.ThrowSound, false, Engine::SoundLibrary::Get(magicBallComp.ThrowSound)->GetVolume());
				sound2DComp.Sound->setPlaybackSpeed(5.5);
			}
		}
	}

	return false;
}

bool HeroScript::OnMouseButtonReleased(Engine::MouseButtonReleasedEvent& e)
{
	if (e.GetMouseButton() == Engine::Mouse::ButtonLeft)
	{
		bool success = StopActive();
	}

	if (e.GetMouseButton() == Engine::Mouse::ButtonRight)
	{
		bool success = StopPassive();
	}

	return false;
}

bool HeroScript::OnMouseScrolled(Engine::MouseScrolledEvent& e)
{
	// Order: None <-> Fire <-> Lightning <-> Stone <->
	switch (m_ActiveSpell)
	{
	case MagicBallType::None:
		EquipActive(e.GetYOffset() > 0 ? MagicBallType::Fire : MagicBallType::Stone);
		break;
	case MagicBallType::Fire:
		EquipActive(e.GetYOffset() > 0 ? MagicBallType::Lightning : MagicBallType::None);
		break;
	case MagicBallType::Lightning:
		EquipActive(e.GetYOffset() > 0 ? MagicBallType::Stone : MagicBallType::Fire);
		break;
	case MagicBallType::Stone:
		EquipActive(e.GetYOffset() > 0 ? MagicBallType::None : MagicBallType::Lightning);
		break;
	default:
		EquipActive(MagicBallType::None);
	}

	return false;
}

bool HeroScript::OnKeyPressed(Engine::KeyPressedEvent& e)
{
	// Order: None -> Light -> Heal ->
	if (e.GetKeyCode() == Engine::Key::E)
	{
		switch (m_PassiveSpell)
		{
		case MagicBallType::None:
				EquipPassive(MagicBallType::Light);
				StartPassive();
			break;		
		case MagicBallType::Light:
				EquipPassive(MagicBallType::Heal);
				StartPassive();
				break;
		case MagicBallType::Heal:
				EquipPassive(MagicBallType::None);
			break;
		default:
			EquipPassive(MagicBallType::None);
		}
	}

	// Order: None <- Light <- Heal <-
	if (e.GetKeyCode() == Engine::Key::Q)
	{
		switch (m_PassiveSpell)
		{
		case MagicBallType::None:
			EquipPassive(MagicBallType::Heal);
			StartPassive();
			break;
		case MagicBallType::Light:
			EquipPassive(MagicBallType::None);
			break;
		case MagicBallType::Heal:
			EquipPassive(MagicBallType::Light);
			StartPassive();
			break;
		default:
			EquipPassive(MagicBallType::None);
		}
	}

	return false;
}

void HeroScript::UpdateMana(Engine::Timestep ts, bool restore)
{
	if (!HasComponent<HeroComponent>())
		return;

	auto& heroComp = GetComponent<HeroComponent>();

	if (restore)
		heroComp.Mana += 5 * ts;

	heroComp.Mana = std::min(100.0f, heroComp.Mana);
	heroComp.LockedMana = std::max(0.0f, heroComp.LockedMana);

	CharacterManaEvent event(heroComp.Mana);
	Engine::Application::Get().OnEvent(event);
}

void HeroScript::UpdateHealth()
{
	if (!HasComponent<HeroComponent>())
		return;

	static float lastUpdate = -1.0f;

	auto& heroComp = GetComponent<HeroComponent>();

	if (lastUpdate != heroComp.Hitpoints)
	{
		heroComp.Hitpoints = std::min(100.0f, heroComp.Hitpoints);

		CharacterHealthEvent event(heroComp.Hitpoints);
		Engine::Application::Get().OnEvent(event);
		lastUpdate = heroComp.Hitpoints;
	}

}

void HeroScript::EquipActive(MagicBallType type)
{
	if (m_ActiveSpell != MagicBallType::None)
		CancelActive();

	if (type != MagicBallType::None && type != MagicBallType::Stone)
	{
		if (m_ActiveParticleSystem)
			m_ActiveParticleSystem.Destroy();

		m_ActiveParticleSystem = CreateMagicBallParticleSystem(type, s_ActiveHandOffset);
	}
	else if (m_ActiveParticleSystem)
	{
		m_ActiveParticleSystem.Destroy();
		m_ActiveParticleSystem = Engine::Entity();
	}

	m_ActiveSpell = type;

	if (type == MagicBallType::Stone)
		StartActive();
}

bool HeroScript::StartActive()
{
	if (m_ActiveHand)
		return true;

	auto& heroComp = GetComponent<HeroComponent>();
	if (heroComp.Mana >= MagicBallMana(m_ActiveSpell))
	{
		heroComp.LockedMana += MagicBallMana(m_ActiveSpell);
		m_ActiveHand = CreateMagicBall(m_ActiveSpell, s_ActiveHandOffset);
		return true;
	}

	return false;
}

bool HeroScript::UseActive(Engine::Timestep ts)
{
	if (!m_ActiveHand)
		return false;

	if (m_ActiveHand.HasComponent<StoneComponent>())
	{
		m_ActiveHand.GetComponent<StoneComponent>().Charge += ts;
		return true;
	}

	auto& heroComp = GetComponent<HeroComponent>();
	auto& magicBallComp = m_ActiveHand.GetComponent<MagicBallComponent>();

	if (magicBallComp.Progress >= 1)
		return false;

	float newProgress = ts / magicBallComp.CastTime;
	magicBallComp.Progress += newProgress;

	heroComp.Mana -= magicBallComp.Mana * newProgress;
	heroComp.LockedMana -= magicBallComp.Mana * newProgress;

	auto& transformComp = m_ActiveHand.GetComponent<TransformComponent>();
	transformComp.Scale = s_MagicBallScale * magicBallComp.Progress;

	if (magicBallComp.Progress >= 1)
	{
		auto& materialComp = m_ActiveHand.GetComponent<MaterialComponent>();
		auto& pointLightComp = m_ActiveHand.GetComponent<PointLightComponent>();

		switch (m_ActiveSpell)
		{
		case MagicBallType::Fire:
			materialComp.Materials[0] = "MagicBall_Fire_ready";
			pointLightComp.Color *= 1.5;
			break;
		case MagicBallType::Lightning:
			materialComp.Materials[0] = "MagicBall_Lightning_ready";
			pointLightComp.Color *= 1.5;
			break;
		}
	}

	return true;
}

bool HeroScript::StopActive()
{
	if (!m_ActiveHand)
		return false;

	if (m_ActiveSpell == MagicBallType::Stone)
	{
		ThrowStone(m_ActiveHand);
		m_ActiveHand = Engine::Entity();
		StartActive();
		return true;
	}

	auto& magicBallComp = m_ActiveHand.GetComponent<MagicBallComponent>();
	if (magicBallComp.Progress <= 1.0f)
	{
		auto& heroComp = GetComponent<HeroComponent>();
		heroComp.LockedMana -= magicBallComp.Mana * (1.0f - magicBallComp.Progress);
		heroComp.Mana += magicBallComp.Mana * magicBallComp.Progress;

		CancelActive();
		return false;
	}

	Throw(m_ActiveHand);
	m_ActiveHand = Engine::Entity();
	return true;
}

void HeroScript::CancelActive()
{
	if (m_ActiveHand)
		m_ActiveHand.Destroy();

	m_ActiveHand = Engine::Entity();
}

void HeroScript::EquipPassive(MagicBallType type)
{
	if (m_PassiveSpell != MagicBallType::None)
		CancelPassive();

	m_PassiveSpell = type;
}

bool HeroScript::StartPassive()
{
	ASSERT(!m_PassiveHand, "Hand is not empty!");
	m_PassiveHand = CreateMagicBall(m_PassiveSpell, s_PassiveHandOffset);
	return true;
}

bool HeroScript::UsePassive(Engine::Timestep ts)
{
	if (!m_PassiveHand)
		return false;

	if (m_PassiveSpell == MagicBallType::Light)
		return false;

	auto& heroComp = GetComponent<HeroComponent>();
	auto& magicBallComp = m_PassiveHand.GetComponent<MagicBallComponent>();

	float neededMana = magicBallComp.Mana * magicBallComp.CastTime * ts;
	if (heroComp.Mana - heroComp.LockedMana < neededMana)
		return false;

	heroComp.Mana -= neededMana;

	switch (m_PassiveSpell) {
	case MagicBallType::Heal:
		heroComp.Hitpoints += magicBallComp.Effect * ts;
		break;
	}

	return true;
}

bool HeroScript::StopPassive()
{
	if (m_PassiveHand && m_PassiveHand.HasComponent<Sound2DComponent>())
	{
		m_PassiveHand.RemoveComponent<Sound2DComponent>();
		return true;
	}

	return false;
}

void HeroScript::CancelPassive()
{
	if (m_PassiveHand)
		m_PassiveHand.Destroy();

	m_PassiveHand = Engine::Entity();
}

Engine::Entity HeroScript::CreateMagicBall(MagicBallType type, glm::vec3 offset)
{
	static std::random_device rand_dev;
	static std::mt19937 generator(rand_dev());
	static std::uniform_real_distribution<float> distr(0, 6.283);

	if (type == MagicBallType::None)
		return Engine::Entity();

	Engine::Entity ball{ Engine::Factory::CreateEntity(*m_RegistryHandle, "MagicBall"), m_RegistryHandle };
	auto& transformComp = ball.GetComponent<TransformComponent>();

	transformComp.Translation = offset;

	ball.AddComponent<ParentComponent>(m_EntityHandle);
	ball.AddComponent<Unserializable>();

	switch (type)
	{
	case MagicBallType::Light:
		transformComp.Scale = { 0.1f, 0.1f, 0.1f };
		ball.AddComponent<MeshComponent>("Sphere UV");
		ball.AddComponent<MaterialComponent>("MagicBall_Light");
		ball.AddComponent<MagicBallComponent>(
			std::string(), "LightCast", std::string(),
			MagicBallEffect(type), MagicBallMana(type), MagicBallCastTime(type)
		);
		ball.AddComponent<PointLightComponent>(glm::vec3{ 0.85f, 0.7f, 0.4f }, 0.4f, 0.1f, 0.05f);
		break;
	case MagicBallType::Heal:
		transformComp.Scale = { 0.1f, 0.1f, 0.1f };
		ball.AddComponent<MeshComponent>("Sphere UV");
		ball.AddComponent<MaterialComponent>("MagicBall_Heal");
		ball.AddComponent<MagicBallComponent>(
			std::string(), "Healing", std::string(),
			MagicBallEffect(type), MagicBallMana(type), MagicBallCastTime(type)
		);
		ball.AddComponent<PointLightComponent>(glm::vec3{ 0.7f, 0.57f, 0.2f }, 0.8f, 0.2f, 0.1f);
		break;
	case MagicBallType::Fire:
		transformComp.Scale = { 0.001f, 0.001f, 0.001f };
		ball.AddComponent<MeshComponent>("Sphere");
		ball.AddComponent<ShadowComponent>();
		ball.AddComponent<MaterialComponent>("MagicBall_Fire");
		ball.AddNativeScript<MagicBallScript>();
		ball.AddComponent<MagicBallComponent>(
			"FireballCast", "FireballShoot", "Impact",
			MagicBallEffect(type), MagicBallMana(type), MagicBallCastTime(type)
		);
		ball.GetComponent<NativeScriptComponent>().Active = false;
		ball.AddComponent<PointLightComponent>(glm::vec3{ 0.5f, 0.3f, 0.1f }, 0.4f, 0.1f, 0.05f);
		break;
	case MagicBallType::Lightning:
		transformComp.Scale = { 0.001f, 0.001f, 0.001f };
		ball.AddComponent<MeshComponent>("Sphere");
		ball.AddComponent<ShadowComponent>();
		ball.AddComponent<MaterialComponent>("MagicBall_Lightning");
		ball.AddNativeScript<MagicBallScript>();
		ball.AddComponent<MagicBallComponent>(
			"LightningCast", "LightningShoot", "Thunder",
			MagicBallEffect(type), MagicBallMana(type), MagicBallCastTime(type)
		);
		ball.GetComponent<NativeScriptComponent>().Active = false;
		ball.AddComponent<PointLightComponent>(glm::vec3{ 0.3f, 0.3f, 0.5f }, 0.4f, 0.1f, 0.05f);
		break;
	case MagicBallType::Stone:
		transformComp.Scale = { 0.13f, 0.2f, 0.13f };
		transformComp.Rotation = { distr(generator), distr(generator), distr(generator) };
		ball.AddComponent<MeshComponent>("Rock_1");
		ball.AddComponent<ShadowComponent>();
		ball.AddComponent<MaterialComponent>("RockMaterial");
		ball.AddComponent<PhysicsMaterialComponent>(0.7, 0.6, 0.1);
		ball.AddComponent<StoneComponent>();
		break;
	default:
		return Engine::Entity();
	}

	if (type != MagicBallType::Stone)
	{
		auto& magicBallComp = ball.GetComponent<MagicBallComponent>();
		if (!magicBallComp.CastSound.empty())
		{
			auto& sound2DComp = ball.EmplaceOrReplace<Engine::Component::Audio::Sound2DComponent>(magicBallComp.CastSound, false, Engine::SoundLibrary::Get(magicBallComp.CastSound)->GetVolume());

			if(type == MagicBallType::Fire)
				sound2DComp.Sound->setPlaybackSpeed(.5);
		}
	}

	return ball;
}

Engine::Entity HeroScript::CreateMagicBallParticleSystem(MagicBallType type, glm::vec3 offset)
{

	Engine::Entity particleSystem{ Engine::Factory::CreateEntity(*m_RegistryHandle, "ParticleSystem (MagicBall)"), m_RegistryHandle };
	auto& transformComp = particleSystem.GetComponent<TransformComponent>();

	transformComp.Translation = offset;

	particleSystem.AddComponent<ParentComponent>(m_EntityHandle);
	particleSystem.AddComponent<Unserializable>();

	switch (type)
	{
	case MagicBallType::Fire:
		particleSystem.AddComponent<ParticleSystemComponent>(Engine::ParticleSystemType::MagicBall, 0.00163f, 0.4649f, 0.0080f, glm::vec4(1, 1, 0, 1), glm::vec4(1, 0, 0, 0));
		break;
	case MagicBallType::Lightning:
		particleSystem.AddComponent<ParticleSystemComponent>(Engine::ParticleSystemType::MagicBall, 0.00163f, 0.4649f, 0.0080f, glm::vec4(0, 1, 1, 1), glm::vec4(1, 1, 0, 0));
		break;
	default:
		return Engine::Entity();
	}

	return particleSystem;
}

void HeroScript::Throw(Engine::Entity ball)
{
	Engine::System::Util::MakeIndependent(*m_RegistryHandle, ball);

	auto& transformComp = GetComponent<TransformComponent>();
	auto& transformComp_ball = ball.GetComponent<TransformComponent>();
	auto& magicBallComp = ball.GetComponent<MagicBallComponent>();

	auto& heroComp = GetComponent<HeroComponent>();

	if (!magicBallComp.ThrowSound.empty())
	{
		ball.EmplaceOrReplace<Engine::Component::Audio::Sound2DComponent>(magicBallComp.ThrowSound, false, Engine::SoundLibrary::Get(magicBallComp.ThrowSound)->GetVolume());
	}

	glm::vec4 velocity = glm::toMat4(glm::quat(transformComp.Rotation)) * glm::vec4{ 0.0f, 0.0f, -28.0f, 0.0 };

	auto* actor = ActorFactory::MagicBall(transformComp_ball.Translation, transformComp_ball.Rotation, transformComp_ball.Scale.x * 3);

	ball.AddComponent<RigidDynamicComponent>(actor);
	ball.AddComponent<KinematicMovementComponent>(glm::vec3{ velocity.x, velocity.y, velocity.z });
	ball.GetComponent<NativeScriptComponent>().Active = true;
}

void HeroScript::ThrowStone(Engine::Entity stone)
{
	Engine::System::Util::MakeIndependent(*m_RegistryHandle, stone);
	DynamicConvexComponent& dynamicComp = stone.AddComponent<DynamicConvexComponent>();

	auto& transformComp = GetComponent<TransformComponent>();
	auto& stoneComp = stone.GetComponent<StoneComponent>();
	float charge = std::min(1.0f, stoneComp.Charge);
	glm::vec4 velocity = glm::toMat4(glm::quat(transformComp.Rotation)) * glm::vec4{ 0.0f, 0.0f, -20.0f * charge, 0.0 };

	dynamicComp.Actor->setLinearVelocity({ velocity.x, velocity.y, velocity.z });

}

