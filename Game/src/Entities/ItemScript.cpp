#include "ItemScript.h"
#include "Components/GameComponents.h"

using TransformComponent = Engine::Component::Core::TransformComponent;

void ItemScript::OnUpdate(Engine::Timestep ts)
{
	bool healthItem = HasComponent<HealthItemComponent>();
	bool manaItem = HasComponent<ManaItemComponent>();

	if (!healthItem && ! manaItem)
		return;

	// Get Hero
	auto view = m_RegistryHandle->view<HeroComponent>();
	if (view.begin() == view.end())
		return;

	Engine::Entity hero{ *view.begin(), m_RegistryHandle };

	TransformComponent worldTransformComp_Item = Engine::System::Util::GlobalTransform(*m_RegistryHandle, m_EntityHandle);
	TransformComponent worldTransformComp_Hero = Engine::System::Util::GlobalTransform(*m_RegistryHandle, hero);
	float distanceToHero = glm::length(worldTransformComp_Item.Translation - worldTransformComp_Hero.Translation);

	if (distanceToHero < 1.9 && worldTransformComp_Item.Translation.y < worldTransformComp_Hero.Translation.y)
	{
		Destroy();
		if (healthItem)
			hero.GetComponent<HeroComponent>().Hitpoints += 30;

		if (manaItem)
			hero.GetComponent<HeroComponent>().Mana += 30;

		Engine::SoundLibrary::Get("CollectItem")->Play2D(false, false, false);
	}

}