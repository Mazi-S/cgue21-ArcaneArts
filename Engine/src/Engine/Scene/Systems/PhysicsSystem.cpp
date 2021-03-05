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

	void OnUpdateKinematic(entt::registry& registry, Timestep ts)
	{
		auto view = registry.view<RigidDynamicComponent, KinematicComponent, KinematicMovementComponent, TransformComponent>();
		for (const entt::entity e : view)
		{
			auto& [rdc, kmc, tc] = view.get<RigidDynamicComponent, KinematicMovementComponent, TransformComponent>(e);

			tc.Translation += kmc.Movement * float(ts);
			tc.Rotation = kmc.Rotation;

			physx::PxTransform transform({ tc.Translation.x, tc.Translation.y, tc.Translation.z }, Util::Math::ToQuaternion(tc.Rotation));
			rdc.Actor->setKinematicTarget(transform);
		}
	}


}

