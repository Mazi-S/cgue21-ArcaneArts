#include "Monster.h"
#include "Components/GameComponents.h"
#include "Events/CharacterHealthEvent.h"

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
		if (monsterComp.DeathSound != nullptr)
		{
			auto& transformComp = GetComponent<TransformComponent>();
			Engine::SoundEngine::Play3D(monsterComp.DeathSound, transformComp.Translation);
		}
		Destroy();
		return;
	}

	if (monsterComp.LiveSound != nullptr)
	{
		auto& transformComp = GetComponent<TransformComponent>();
		//Engine::SoundEngine::Play3D(monsterComp.LiveSound, transformComp.Translation, true);
	}

	// Get Hero
	auto view = m_RegistryHandle->view<HeroComponent>();
	ASSERT(view.begin() != view.end(), "No Character found!");
	Engine::Entity hero{ *view.begin(), m_RegistryHandle };

	float speed = 1;

	float distanceToHero = glm::length(GetComponent<TransformComponent>().Translation - hero.GetComponent<TransformComponent>().Translation);
	if (distanceToHero < monsterComp.HitRange)
	{
		auto& heroComp = hero.GetComponent<HeroComponent>();
		heroComp.Hitpoints -= monsterComp.Damage;
		
		CharacterHealthEvent event(heroComp.Hitpoints);
		Engine::Application::Get().OnEvent(event);
		speed = -200;
	}

	if (distanceToHero < monsterComp.ViewRange)
	{
		auto& characterTransformComponent = hero.GetComponent<TransformComponent>();
		auto& monsterTransformComponent = GetComponent<TransformComponent>();

		glm::vec3 movement = glm::normalize(characterTransformComponent.Translation - monsterTransformComponent.Translation) * monsterComp.Speed * speed;
		glm::quat rotation = glm::quatLookAt(glm::normalize(-movement), { 0,1,0 });
		EmplaceOrReplace<KinematicMovementComponent>(movement, rotation);
	}
}
