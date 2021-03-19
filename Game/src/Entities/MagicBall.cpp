#include "MagicBall.h"
#include "Components/GameComponents.h"

using HitComponent = Engine::Component::Physics::HitComponent;
using TransformComponent = Engine::Component::Core::TransformComponent;

void MagicBall::OnUpdate(Engine::Timestep ts)
{
	if (!HasComponent<MagicBallComponent>())
		return;

	auto& magicBallComp = GetComponent<MagicBallComponent>();

	if (HasComponent<HitComponent>())
	{
		auto& hitComp = GetComponent<HitComponent>();
		Engine::Entity hitEntity(hitComp.Other, m_RegistryHandle);

		if (hitEntity.HasComponent<MonsterComponent>())
		{
			hitEntity.GetComponent<MonsterComponent>().Hitpoints -= magicBallComp.Damage;
		}

		if (magicBallComp.ImpactSound != nullptr)
		{
			auto& transformComp = GetComponent<TransformComponent>();
			Engine::SoundEngine::Play3D(magicBallComp.ImpactSound, transformComp.Translation);
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
