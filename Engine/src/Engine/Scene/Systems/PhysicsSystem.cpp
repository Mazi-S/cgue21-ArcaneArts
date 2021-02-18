#include "egpch.h"
#include "PhysicsSystem.h"
#include "Engine/Scene/Components.h"
#include "Engine/Util/Math.h"

#include "PxPhysicsAPI.h"

namespace Engine::System::Physics {

	void OnUpdate(entt::registry& registry)
	{
		// Update Regid Dynamic
		{
			auto view = registry.view<RegidDynamicComponent, TransformComponent>();
			for (const entt::entity e : view)
			{
				auto& [rdc, tc] = view.get<RegidDynamicComponent, TransformComponent>(e);

				physx::PxTransform t = rdc.Actor->getGlobalPose();
				tc.Translation = { t.p.x, t.p.y, t.p.z };
				tc.Rotation = Engine::Util::Math::ToEulerAngles(t.q);
			}
		}

		// Update Character Controller
		{
			auto view = registry.view<CharacterControllerComponent, TransformComponent>();
			for (const entt::entity e : view)
			{
				auto& [ccc, tc] = view.get<CharacterControllerComponent, TransformComponent>(e);

				const physx::PxExtendedVec3& pos = ccc.Controller->getPosition();
				tc.Translation = { pos.x, pos.y, pos.z };
			}
		}
	}

}

