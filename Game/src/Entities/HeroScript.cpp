#include "HeroScript.h"
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

static glm::vec3 s_ActiveHandOffset = { 0.42f, 0.68f, -1.0f };
static glm::vec3 s_PassiveHandOffset = { -0.5f, 0.65f, -1.0f };

static glm::vec3 s_MagicBallScale = { 0.1f, 0.1f, 0.1f };

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

	//if (e.GetMouseButton() == Engine::Mouse::ButtonRight)
	//{
	//	bool created = StartPassive();
	//	//TODO: play sound if failed!!!
	//}

	return false;
}

bool HeroScript::OnMouseButtonReleased(Engine::MouseButtonReleasedEvent& e)
{
	if (e.GetMouseButton() == Engine::Mouse::ButtonLeft)
	{
		bool success = StopActive();
		//TODO: play sound if failed!!!
	}

	//if (e.GetMouseButton() == Engine::Mouse::ButtonRight)
	//{
	//	bool success = StopPassive();
	//	//TODO: play sound if failed!!!
	//}

	return false;
}

bool HeroScript::OnMouseScrolled(Engine::MouseScrolledEvent& e)
{
	// Order: None <-> Fire <-> Lightning <->
	switch (m_ActiveSpell)
	{
	case MagicBallType::None:
		EquipActive(e.GetYOffset() > 0 ? MagicBallType::Fire : MagicBallType::Lightning);
		break;
	case MagicBallType::Fire:
		EquipActive(e.GetYOffset() > 0 ? MagicBallType::Lightning : MagicBallType::None);
		break;
	case MagicBallType::Lightning:
		EquipActive(e.GetYOffset() > 0 ? MagicBallType::None : MagicBallType::Fire);
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
				StartPassive();
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
			StartPassive();
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

	// TODO: start particle system
	m_ActiveSpell = type;
}

bool HeroScript::StartActive()
{
	ASSERT(!m_ActiveHand, "Hand is not empty!");

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

		switch (m_ActiveSpell)
		{
		case MagicBallType::Fire:
			materialComp.Material = Engine::MaterialLibrary::Get("MagicBall_Fire_ready");
			break;
		case MagicBallType::Lightning:
			materialComp.Material = Engine::MaterialLibrary::Get("MagicBall_Lightning_ready");
			break;
		}
	}

	return true;
}

bool HeroScript::StopActive()
{
	if (!m_ActiveHand)
		return false;

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

	// TODO: start particle system
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
	Engine::Entity ball{ Engine::Factory::CreateEntity(*m_RegistryHandle, "MagicBall"), m_RegistryHandle };
	auto& transformComp = ball.GetComponent<TransformComponent>();

	transformComp.Translation = offset;

	ball.AddComponent<ParentComponent>(m_EntityHandle);
	ball.AddComponent<MeshComponent>(Engine::MeshLibrary::Get("Sphere"));

	switch (type)
	{
	case MagicBallType::Light:
		transformComp.Scale = { 0.1f, 0.1f, 0.1f };
		ball.AddComponent<MaterialComponent>(Engine::MaterialLibrary::Get("MagicBall_Light"));
		ball.AddComponent<MagicBallComponent>(
			nullptr, Engine::SoundLibrary::Get("LightCast"), nullptr,
			MagicBallEffect(type), MagicBallMana(type), MagicBallCastTime(type)
		);
		ball.AddComponent<PointLightComponent>(glm::vec3{ 0.85f, 0.7f, 0.4f }, 0.4f, 0.1f, 0.05f);
		break;
	case MagicBallType::Heal:
		transformComp.Scale = { 0.1f, 0.1f, 0.1f };
		ball.AddComponent<MaterialComponent>(Engine::MaterialLibrary::Get("MagicBall_Heal"));
		ball.AddComponent<MagicBallComponent>(
			nullptr, nullptr, nullptr,
			MagicBallEffect(type), MagicBallMana(type), MagicBallCastTime(type)
		);
		ball.AddComponent<PointLightComponent>(glm::vec3{ 0.7f, 0.57f, 0.2f }, 0.8f, 0.2f, 0.1f);
		break;
	case MagicBallType::Fire:
		transformComp.Scale = { 0.001f, 0.001f, 0.001f };
		ball.AddComponent<ShadowComponent>();
		ball.AddComponent<MaterialComponent>(Engine::MaterialLibrary::Get("MagicBall_Fire"));
		ball.AddNativeScript<MagicBallScript>();
		ball.AddComponent<MagicBallComponent>(
			nullptr, Engine::SoundLibrary::Get("FireballShoot"), Engine::SoundLibrary::Get("Impact"),
			MagicBallEffect(type), MagicBallMana(type), MagicBallCastTime(type)
		);
		ball.GetComponent<NativeScriptComponent>().Active = false;
		break;
	case MagicBallType::Lightning:
		transformComp.Scale = { 0.001f, 0.001f, 0.001f };
		ball.AddComponent<ShadowComponent>();
		ball.AddComponent<MaterialComponent>(Engine::MaterialLibrary::Get("MagicBall_Lightning"));
		ball.AddNativeScript<MagicBallScript>();
		ball.AddComponent<MagicBallComponent>(
			Engine::SoundLibrary::Get("LightningShoot"), nullptr, Engine::SoundLibrary::Get("Thunder"),
			MagicBallEffect(type), MagicBallMana(type), MagicBallCastTime(type)
		);
		ball.GetComponent<NativeScriptComponent>().Active = false;
		break;
	default:
		return Engine::Entity();
	}

	auto& magicBallComp = ball.GetComponent<MagicBallComponent>();
	if (magicBallComp.CastSound != nullptr)
		Engine::SoundEngine::Get()->play2D(magicBallComp.CastSound);

	return ball;
}

void HeroScript::Throw(Engine::Entity ball)
{
	Engine::System::Util::MakeIndependent(*m_RegistryHandle, ball);

	auto& transformComp = GetComponent<TransformComponent>();
	auto& transformComp_ball = ball.GetComponent<TransformComponent>();
	auto& magicBallComp = ball.GetComponent<MagicBallComponent>();

	auto& heroComp = GetComponent<HeroComponent>();

	if (magicBallComp.ThrowSound != nullptr)
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



















#if 0

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

void HeroScript::DropLeft()
{
	m_LeftHand.Destroy();
	m_LeftHand = Engine::Entity();
}

void HeroScript::DropRight()
{
	m_RightHand.Destroy();
	m_RightHand = Engine::Entity();
}

Engine::Entity HeroScript::CreateFireBall(glm::vec3 offset)
{
	Engine::Entity ball{ Engine::Factory::CreateEntity(*m_RegistryHandle, "MagicBall"), m_RegistryHandle };
	auto& transformComp = ball.GetComponent<TransformComponent>();

	transformComp.Scale = { 0.001f, 0.001f, 0.001f };
	transformComp.Translation = offset;

	ball.AddComponent<ParentComponent>(m_EntityHandle);
	ball.AddComponent<MeshComponent>(Engine::MeshLibrary::Get("Sphere"));

	ball.AddComponent<ShadowComponent>();
	ball.AddComponent<MaterialComponent>(Engine::MaterialLibrary::Get("MagicBall_Fire"));
	ball.AddNativeScript<MagicBallScript>();
	ball.AddComponent<MagicBallComponent>(nullptr, Engine::SoundLibrary::Get("FireballShoot"), Engine::SoundLibrary::Get("Impact"), 10.0f, 10.0f, .5f);
	ball.GetComponent<NativeScriptComponent>().Active = false;

	auto& magicBallComp = ball.GetComponent<MagicBallComponent>();
	if (magicBallComp.CastSound != nullptr)
		Engine::SoundEngine::Get()->play2D(magicBallComp.CastSound);

	return ball;
}

Engine::Entity HeroScript::CreateLightningBall(glm::vec3 offset)
{
	Engine::Entity ball{ Engine::Factory::CreateEntity(*m_RegistryHandle, "MagicBall"), m_RegistryHandle };
	auto& transformComp = ball.GetComponent<TransformComponent>();

	transformComp.Scale = { 0.001f, 0.001f, 0.001f };
	transformComp.Translation = offset;

	ball.AddComponent<ParentComponent>(m_EntityHandle);
	ball.AddComponent<MeshComponent>(Engine::MeshLibrary::Get("Sphere"));

	ball.AddComponent<ShadowComponent>();
	ball.AddComponent<MaterialComponent>(Engine::MaterialLibrary::Get("MagicBall_Water"));
	ball.AddNativeScript<MagicBallScript>();
	ball.AddComponent<MagicBallComponent>(Engine::SoundLibrary::Get("LightningShoot"), nullptr, Engine::SoundLibrary::Get("Thunder"), 20.0f, 15.0f, 1.5f);
	ball.GetComponent<NativeScriptComponent>().Active = false;

	auto& magicBallComp = ball.GetComponent<MagicBallComponent>();
	if (magicBallComp.CastSound != nullptr)
		Engine::SoundEngine::Get()->play2D(magicBallComp.CastSound);

	return ball;
}


void HeroScript::Light(Engine::Entity ball)
{
	if (ball.HasComponent<PointLightComponent>())
		ball.RemoveComponent<PointLightComponent>();
	else
	{
		ball.AddComponent<PointLightComponent>(glm::vec3{ 0.85f, 0.7f, 0.7f }, 0.4f, 0.1f, 0.05f);
	}
}

void HeroScript::Heal(Engine::Entity ball)
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



bool HeroScript::CreateActive()
{
	ASSERT(!m_LeftHand, "There is already an active spell!");

	auto& heroComp = GetComponent<HeroComponent>();

	switch (m_ActiveSpell)
	{
	case MagicBallType::Fire:
		if (heroComp.Mana - heroComp.LockedMana - 10 > 0)
		{
			heroComp.LockedMana += 10;
			m_LeftHand = CreateFireBall({ 0.42f, 0.68f, -1.0f });
			return true;
		}
		return false;
	case MagicBallType::Lightning:
		if (heroComp.Mana - heroComp.LockedMana - 15 > 0)
		{
			heroComp.LockedMana += 15;
			m_LeftHand = CreateLightningBall({ 0.42f, 0.68f, -1.0f });
			return true;
		}
		return false;
	}

	return false;
}

bool HeroScript::CastActive(Engine::Timestep ts)
{
	if (!m_LeftHand)
		return false;

	auto& heroComp = GetComponent<HeroComponent>();
	auto& magicBallComp = m_LeftHand.GetComponent<MagicBallComponent>();

	magicBallComp.Cast += ts;

	if (magicBallComp.Cast < magicBallComp.CastTime)
	{
		auto& magicBall_TransformComp = m_LeftHand.GetComponent<TransformComponent>();
		magicBall_TransformComp.Scale = glm::vec3{ 0.1f, 0.1f, 0.1f } * (magicBallComp.Cast / magicBallComp.CastTime);

		float mana = (magicBallComp.Mana / magicBallComp.CastTime) * ts;
		heroComp.Mana -= mana;
		heroComp.LockedMana -= mana;

		CharacterManaEvent event(heroComp.Mana);
		Engine::Application::Get().OnEvent(event);
	}

	return true;
}

void HeroScript::ReleaseActive()
{
	if (!m_LeftHand)
		return;

	auto& magicBallComp = m_LeftHand.GetComponent<MagicBallComponent>();
	if (magicBallComp.Cast >= magicBallComp.CastTime)
	{
		Throw(m_LeftHand);
		m_LeftHand = Engine::Entity();
	}
	else
	{
		auto& heroComp = GetComponent<HeroComponent>();

		float cast = magicBallComp.Cast / magicBallComp.CastTime;

		heroComp.Mana += magicBallComp.Mana * cast;
		heroComp.LockedMana -= magicBallComp.Mana * (1 - cast);

		CharacterManaEvent event(heroComp.Mana);
		Engine::Application::Get().OnEvent(event);
		DropLeft();
	}


}
#endif
