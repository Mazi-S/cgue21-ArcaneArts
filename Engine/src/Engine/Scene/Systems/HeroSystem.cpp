#include "egpch.h"
#include "HeroSystem.h"

#include "Util.h"

#include "Engine/Scene/Components.h"

namespace Engine::System::Hero {

#if 0
	void CreateMagicBall(entt::registry registry, entt::entity hero, MagicBallType type)
	{

	}

	void ThrowLeft(entt::registry& registry, entt::entity hero)
	{
		auto& [hc, tc] = registry.try_get<HeroComponent, TransformComponent>(hero);

		if (hc == nullptr || hc->LeftHand == entt::null)
			return;

		entt::entity item = hc->LeftHand;
		hc->LeftHand = entt::null;

		TransformComponent& tc_item = registry.get<TransformComponent>(item);
		glm::vec4 positionWorld = Util::Transform(registry, hero) * glm::vec4{ tc_item.Translation.x, tc_item.Translation.y, tc_item.Translation.z, 1.0f };
		tc_item.Translation.x = positionWorld.x;
		tc_item.Translation.y = positionWorld.y;
		tc_item.Translation.z = positionWorld.z;
		tc_item.Rotation += tc->Rotation;
		tc_item.Scale *= tc->Scale;

		registry.remove<ParentComponent>(item);
	}

	void ThrowRight(entt::registry& registry, entt::entity hero)
	{
		auto& [hc, tc] = registry.try_get<HeroComponent, TransformComponent>(hero);

		if (hc == nullptr || hc->RightHand == entt::null)
			return;

		entt::entity item = hc->RightHand;
		hc->RightHand = entt::null;

		TransformComponent& tc_item = registry.get<TransformComponent>(item);
		glm::vec4 positionWorld = Util::Transform(registry, hero) * glm::vec4{ tc_item.Translation.x, tc_item.Translation.y, tc_item.Translation.z, 1.0f };
		tc_item.Translation.x = positionWorld.x;
		tc_item.Translation.y = positionWorld.y;
		tc_item.Translation.z = positionWorld.z;
		tc_item.Rotation += tc->Rotation;
		tc_item.Scale *= tc->Scale;

		registry.remove<ParentComponent>(item);
	}
#endif // 0

}