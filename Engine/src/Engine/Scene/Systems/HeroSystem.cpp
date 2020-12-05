#include "egpch.h"
#include "HeroSystem.h"

#include "Engine/Scene/Components.h"

namespace Engine::System::Hero {

	void Throw(entt::registry& registry, entt::entity hero, bool rightHand)
	{
		auto* hc = registry.try_get<HeroComponent>(hero);

		if (hc == nullptr || (rightHand && hc->RightHand == entt::null) || (!rightHand && hc->LeftHand == entt::null))
			return;

		// todo
		if (rightHand)
		{
			registry.destroy(hc->RightHand);
			hc->RightHand = entt::null;
		}
		else
		{
			registry.destroy(hc->LeftHand);
			hc->LeftHand = entt::null;
		}
	}

}