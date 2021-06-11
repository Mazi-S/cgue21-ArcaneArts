#include "MonsterScript.h"
#include "Components/GameComponents.h"
#include "Events/CharacterHealthEvent.h"
#include "Events/MonsterDied.h"

using TransformComponent			= Engine::Component::Core::TransformComponent;
using KinematicMovementComponent	= Engine::Component::Physics::KinematicMovementComponent;
using RigidDynamicComponent			= Engine::Component::Physics::RigidDynamicComponent;
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

	TransformComponent worldTransformComp_Monster = Engine::System::Util::GlobalTransform(*m_RegistryHandle, m_EntityHandle);

	if (!monsterComp.LiveSound.empty())
	{
		// Engine::SoundLibrary::Get(monsterComp.LiveSound)->Play3D(worldTransformComp_Monster.Translation, true, true);
	}

	// Get Hero
	auto view = m_RegistryHandle->view<HeroComponent>();
	if (view.begin() == view.end())
		return;

	Engine::Entity hero{ *view.begin(), m_RegistryHandle };

	TransformComponent worldTransformComp_Hero = Engine::System::Util::GlobalTransform(*m_RegistryHandle, hero);

	float distanceToHero = glm::length(worldTransformComp_Monster.Translation - worldTransformComp_Hero.Translation);

	if (distanceToHero < monsterComp.HitRange)
	{
		auto& heroComp = hero.GetComponent<HeroComponent>();
		heroComp.Hitpoints -= monsterComp.Damage;
		
		CharacterHealthEvent event(heroComp.Hitpoints);
		Engine::Application::Get().OnEvent(event);

		if (HasComponent<RigidDynamicComponent>())
		{
			RigidDynamicComponent& rigidDynamicComp = GetComponent<RigidDynamicComponent>();

			if (rigidDynamicComp.Actor != nullptr)
			{
				glm::vec3 movement = glm::normalize(worldTransformComp_Hero.Translation - worldTransformComp_Monster.Translation) * -5.0f;
				glm::quat rotation = glm::quatLookAt(glm::normalize(-movement), { 0,1,0 });

				GetComponent<TransformComponent>().Translation += movement;
				worldTransformComp_Monster = Engine::System::Util::GlobalTransform(*m_RegistryHandle, m_EntityHandle);

				physx::PxTransform transform({ worldTransformComp_Monster.Translation.x, worldTransformComp_Monster.Translation.y, worldTransformComp_Monster.Translation.z }, physx::PxQuat(rotation.x, rotation.y, rotation.z, rotation.w));
				rigidDynamicComp.Actor->setKinematicTarget(transform);
			}
		}
	}

	if (distanceToHero < monsterComp.ViewRange)
	{
		glm::vec3 movement = glm::normalize(worldTransformComp_Hero.Translation - worldTransformComp_Monster.Translation) * monsterComp.Speed;
		glm::quat rotation = glm::quatLookAt(glm::normalize(-movement), { 0,1,0 });
		EmplaceOrReplace<KinematicMovementComponent>(movement, rotation);
	}
	else if(HasComponent<KinematicMovementComponent>())
	{
		RemoveComponent<KinematicMovementComponent>();
	}
}
