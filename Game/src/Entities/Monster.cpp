#include "Monster.h"

void Monster::OnUpdate(Engine::Timestep ts)
{
	if (HasComponent<Engine::MagicBallHitComponent>())
	{
		m_Hitpoints -= 20.0f;
		RemoveComponent<Engine::MagicBallHitComponent>();
	}

	Engine::Entity character;
	auto view = m_RegistryHandle.view<Engine::CharacterControllerComponent, Engine::TransformComponent>();
	for (const entt::entity e : view)
		character = Engine::Entity(e, m_Scene);

	auto characterTransformComponent = character.GetComponent<Engine::TransformComponent>();
	auto monsterTransformComponent = GetComponent<Engine::TransformComponent>();
	

	if (HasComponent<Engine::KinematicMovementComponent>()) {
		RemoveComponent<Engine::KinematicMovementComponent>();
		AddComponent<Engine::KinematicMovementComponent>(glm::normalize(characterTransformComponent.Translation - monsterTransformComponent.Translation) * 2.0f);
	}

	if (m_Hitpoints <= 0.0f)
	{
		Destroy();
		return;
	}
}
