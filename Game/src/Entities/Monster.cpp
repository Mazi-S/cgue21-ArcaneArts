#include "Monster.h"
#include "Components/GameComponents.h"

using TransformComponent			= Engine::Component::Core::TransformComponent;
using KinematicMovementComponent	= Engine::Component::Physics::KinematicMovementComponent;
using CharacterControllerComponent	= Engine::Component::Physics::CharacterControllerComponent;

void Monster::OnUpdate(Engine::Timestep ts)
{
	if (!HasComponent<MonsterComponent>())
		return;

	auto& monsterComp = GetComponent<MonsterComponent>();

	if (monsterComp.Hitpoints <= 0.0f)
	{
		Destroy();
		return;
	}

	auto view = m_RegistryHandle->view<CharacterControllerComponent, TransformComponent>();
	ASSERT(view.begin() != view.end(), "No Character found!");
	Engine::Entity character{ *view.begin(), m_RegistryHandle };
	auto characterTransformComponent = character.GetComponent<TransformComponent>();
	auto monsterTransformComponent = GetComponent<TransformComponent>();
	glm::vec3 movement = glm::normalize(characterTransformComponent.Translation - monsterTransformComponent.Translation) * monsterComp.Speed;
	glm::quat rotation = glm::quatLookAt(glm::normalize(-movement), {0,1,0});
	EmplaceOrReplace<KinematicMovementComponent>(movement, rotation);
}
