#include "MagicBall.h"

void MagicBall::OnUpdate(Engine::Timestep ts)
{
	m_Lifetime -= ts;

	if (m_Lifetime < 0.0f)
	{
		Destroy();
		return;
	}

	if (HasComponent<Engine::HitComponent>())
	{
		auto& tshc = GetComponent<Engine::HitComponent>();
		Engine::Entity hitEntity(tshc.Other, m_Scene);
		hitEntity.Destroy();
		Destroy();
		return;
	}
}
