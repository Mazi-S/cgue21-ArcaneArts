#include "MagicBall.h"

void MagicBall::OnUpdate(Engine::Timestep ts)
{
	m_Lifetime -= ts;

	if (m_Lifetime < 0.0f)
	{
		Destroy();
		return;
	}

	if (HasComponent<Engine::Component::Physics::HitComponent>())
	{
		auto& hc = GetComponent<Engine::Component::Physics::HitComponent>();
		Engine::Entity hitEntity(hc.Other, m_Scene);
		if (hitEntity.HasComponent<Engine::MonsterComponent>())
			hitEntity.AddComponent<Engine::MagicBallHitComponent>();
		Destroy();
		return;
	}
}
