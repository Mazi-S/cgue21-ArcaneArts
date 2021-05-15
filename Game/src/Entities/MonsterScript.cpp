#include "MonsterScript.h"
#include "Components/GameComponents.h"
#include "Events/CharacterHealthEvent.h"
#include "Events/MonsterDied.h"

using TransformComponent			= Engine::Component::Core::TransformComponent;
using KinematicMovementComponent	= Engine::Component::Physics::KinematicMovementComponent;
using CharacterControllerComponent	= Engine::Component::Physics::CharacterControllerComponent;

void MonsterScript::OnUpdate(Engine::Timestep ts)
{
	if (!HasComponent<MonsterComponent>())
		return;

	auto& monsterComp = GetComponent<MonsterComponent>();

	if (monsterComp.Hitpoints <= 0.0f)
	{
		if (!monsterComp.DeathSound.empty())
		{
			auto& transformComp = GetComponent<TransformComponent>();
			Engine::SoundLibrary::Get(monsterComp.DeathSound)->Play3D(transformComp.Translation);
		}
		Engine::Application::Get().OnEvent(MonsterDiedEvent());
		Destroy();
		return;
	}

	if (!monsterComp.LiveSound.empty())
	{
		auto& transformComp = GetComponent<TransformComponent>();
		//Engine::SoundLibrary::Get(monsterComp.LiveSound)->Play3D(transformComp.Translation, true, true);
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
	else if(HasComponent<KinematicMovementComponent>())
	{
		RemoveComponent<KinematicMovementComponent>();
	}
}
