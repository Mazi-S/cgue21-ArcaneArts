#include "MagicBall.h"
#include "Components/GameComponents.h"

using CollisionEventComponent	= Engine::Component::Event::CollisionEventComponent;
using TransformComponent		= Engine::Component::Core::TransformComponent;

void MagicBall::OnUpdate(Engine::Timestep ts)
{
	if (!HasComponent<MagicBallComponent>())
		return;

	auto& magicBallComp = GetComponent<MagicBallComponent>();

	if (HasComponent<CollisionEventComponent>())
	{
		auto& hitComp = GetComponent<CollisionEventComponent>();
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
