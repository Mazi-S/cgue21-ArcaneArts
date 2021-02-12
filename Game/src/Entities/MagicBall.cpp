#include "MagicBall.h"

void MagicBall::OnUpdate(Engine::Timestep ts)
{
	//m_Lifetime -= ts;

	if (m_Lifetime < 0.3f)
	{
		glm::vec3 scale = glm::vec3(2.0f + 10.0f * m_Lifetime);
		GetComponent<Engine::TransformComponent>().Scale = scale;
		GetComponent<Engine::MaterialComponent>().Material = Engine::MaterialLibrary::Get("MagicBallRed");
	}

	if (m_Lifetime < 0.0f)
		Destroy();
}
