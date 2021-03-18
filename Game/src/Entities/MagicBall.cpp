#include "MagicBall.h"
#include "Components/GameComponents.h"

using HitComponent = Engine::Component::Physics::HitComponent;

void MagicBall::OnUpdate(Engine::Timestep ts)
{
	if (!HasComponent<MagicBallComponent>())
		return;

	auto& magicBallComp = GetComponent<MagicBallComponent>();

	if (HasComponent<HitComponent>())
	{
		auto& hitComp = GetComponent<HitComponent>();
		Engine::Entity hitEntity(hitComp.Other, m_Scene);

		if (hitEntity.HasComponent<MonsterComponent>())
		{
			hitEntity.GetComponent<MonsterComponent>().Hitpoints -= magicBallComp.Damage;
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
