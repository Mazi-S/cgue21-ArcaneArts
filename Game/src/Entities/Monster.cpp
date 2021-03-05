#include "Monster.h"

void Monster::OnUpdate(Engine::Timestep ts)
{
	if (HasComponent<Engine::MagicBallHitComponent>())
	{
		m_Hitpoints -= 20.0f;
		RemoveComponent<Engine::MagicBallHitComponent>();
	}

	auto view = m_RegistryHandle.view<Engine::CharacterControllerComponent, Engine::TransformComponent>();
	ASSERT(view.begin() != view.end(), "No Character found!");
	Engine::Entity character{ *view.begin(), m_Scene };

	auto characterTransformComponent = character.GetComponent<Engine::TransformComponent>();
	auto monsterTransformComponent = GetComponent<Engine::TransformComponent>();

	if (HasComponent<Engine::KinematicMovementComponent>()) {
		RemoveComponent<Engine::KinematicMovementComponent>();
	}

	glm::vec3 movement = glm::normalize(characterTransformComponent.Translation - monsterTransformComponent.Translation) * 2.0f;
	glm::quat rotation = glm::quatLookAt(glm::normalize(-movement), {0,1,0});

	AddComponent<Engine::KinematicMovementComponent>(movement, rotation);

	if (m_Hitpoints <= 0.0f)
	{
		Destroy();
		return;
	}
}
