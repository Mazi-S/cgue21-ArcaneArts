#include "egpch.h"
#include "PhysicsSystem.h"
#include "Engine/Scene/Components.h"
#include "Engine/Util/Math.h"

#include "PxPhysicsAPI.h"

namespace Engine::System::Physics {

	void OnUpdateKinematic(entt::registry& registry, Timestep ts)
	{
		{
			auto view = registry.view<RigidDynamicComponent, KinematicMovementComponent, TransformComponent>();
			for (const entt::entity e : view)
			{
				auto& [rdc, kc, tc] = view.get<RigidDynamicComponent, KinematicMovementComponent, TransformComponent>(e);

				tc.Translation += kc.Movement * float(ts);
				rdc.Actor->setKinematicTarget({ tc.Translation.x, tc.Translation.y, tc.Translation.z });
			}
		}
		{
			auto view = registry.view<RigidKinematicComponent, KinematicMovementComponent, TransformComponent>();
			for (const entt::entity e : view)
			{
				auto& [rkc, kc, tc] = view.get<RigidKinematicComponent, KinematicMovementComponent, TransformComponent>(e);

				tc.Translation += kc.Movement * float(ts);
				rkc.Actor->setKinematicTarget({ tc.Translation.x, tc.Translation.y, tc.Translation.z });
			}
		}
	}

	void OnUpdate(entt::registry& registry)
	{
		// Update Regid Dynamic
		{
			auto view = registry.view<RigidDynamicComponent, TransformComponent>();
			for (const entt::entity e : view)
			{
				auto& [rdc, tc] = view.get<RigidDynamicComponent, TransformComponent>(e);

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

