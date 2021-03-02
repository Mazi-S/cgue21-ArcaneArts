#include "MagicBall.h"

void MagicBall::OnUpdate(Engine::Timestep ts)
{
	m_Lifetime -= ts;

	if (m_Lifetime < 0.0f)
		Destroy();
}
