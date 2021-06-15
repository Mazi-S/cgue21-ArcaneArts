#include "MagicBallScript.h"
#include "Components/GameComponents.h"

using CollisionEventComponent	= Engine::Component::Event::CollisionEventComponent;
using TransformComponent		= Engine::Component::Core::TransformComponent;

float MagicBallMana(MagicBallType type)
{
	switch (type)
	{
	case MagicBallType::Light:
		return 1.0f;
		break;
	case MagicBallType::Heal:
		return 10.0f;
		break;
	case MagicBallType::Fire:
		return 12.0f;
		break;
	case MagicBallType::Lightning:
		return 18.0f;
		break;
	case MagicBallType::Stone:
		return 0.0f;
		break;
	}

	return 0.0f;
}

float MagicBallCastTime(MagicBallType type)
{
	switch (type)
	{
	case MagicBallType::Light:
		return 1.0f;
		break;
	case MagicBallType::Heal:
		return 1.0f;
		break;
	case MagicBallType::Fire:
		return 1.2f;
		break;
	case MagicBallType::Lightning:
		return 1.8f;
		break;
	}

	return 0.0f;
}

float MagicBallEffect(MagicBallType type)
{
	switch (type)
	{
	case MagicBallType::Heal:
		return 15.0f;
		break;
	case MagicBallType::Fire:
		return 12.0f;
		break;
	case MagicBallType::Lightning:
		return 18.0f;
		break;
	}

	return 0.0f;
}

void MagicBallScript::OnUpdate(Engine::Timestep ts)
{
	if (!HasComponent<MagicBallComponent>())
		return;

	auto& magicBallComp = GetComponent<MagicBallComponent>();

	if (HasComponent<CollisionEventComponent>())
	{
		auto& collisionEventComp = GetComponent<CollisionEventComponent>();
		Engine::Entity collidedEntity(collisionEventComp.Other, m_RegistryHandle);

		if (collidedEntity.HasComponent<MonsterComponent>())
		{
			collidedEntity.GetComponent<MonsterComponent>().Hitpoints -= magicBallComp.Effect;
			collidedEntity.GetComponent<MonsterComponent>().ViewRange += 100;
		}

		if (!magicBallComp.ImpactSound.empty())
		{
			auto& transformComp = GetComponent<TransformComponent>();
			Engine::SoundLibrary::Get(magicBallComp.ImpactSound)->Play3D(transformComp.Translation);
		}

		Destroy();
		return;
	}

	magicBallComp.Lifetime -= ts;
	if (magicBallComp.Lifetime < 0.0f)
	{
		Destroy();
		return;
	}
}
