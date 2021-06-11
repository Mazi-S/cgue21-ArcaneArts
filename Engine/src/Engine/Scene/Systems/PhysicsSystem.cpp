#include "egpch.h"
#include "PhysicsSystem.h"
#include "Engine/Scene/Components.h"
#include "Engine/Util/Math.h"
#include "Util.h"

#include "PxPhysicsAPI.h"

namespace Engine::System::Physics {
	
	void OnUpdate(entt::registry& registry)
	{
		// Update Regid Dynamic
		{
			auto view = registry.view<Component::Physics::RigidDynamicComponent, Component::Core::TransformComponent>();
			for (const entt::entity e : view)
			{
				auto& [rdc, tc] = view.get<Component::Physics::RigidDynamicComponent, Component::Core::TransformComponent>(e);

				physx::PxTransform t = rdc.Actor->getGlobalPose();
				tc.Translation = { t.p.x, t.p.y, t.p.z };
				tc.Rotation = Engine::Util::Math::ToEulerAngles(t.q);
			}
		}

		// Update Character Controller
		{
			auto view = registry.view<Component::Physics::CharacterControllerComponent, Component::Core::TransformComponent>();
			for (const entt::entity e : view)
			{
				auto& [ccc, tc] = view.get<Component::Physics::CharacterControllerComponent, Component::Core::TransformComponent>(e);

				const physx::PxExtendedVec3& pos = ccc.Controller->getPosition();
				tc.Translation = { pos.x, pos.y, pos.z };
			}
		}
	}

	void OnUpdateKinematic(entt::registry& registry, Timestep ts)
	{
		auto view = registry.view<Component::Physics::RigidDynamicComponent, Component::Physics::KinematicMovementComponent, Component::Core::TransformComponent>();
		for (const entt::entity e : view)
		{
			auto& [rdc, kmc, tc] = view.get<Component::Physics::RigidDynamicComponent, Component::Physics::KinematicMovementComponent, Component::Core::TransformComponent>(e);
			tc.Translation += kmc.Movement * float(ts);

			Component::Core::TransformComponent transformComp = Engine::System::Util::GlobalTransform(registry, e);

			physx::PxTransform transform({ transformComp.Translation.x, transformComp.Translation.y, transformComp.Translation.z }, physx::PxQuat(kmc.Rotation.x, kmc.Rotation.y, kmc.Rotation.z, kmc.Rotation.w));
			rdc.Actor->setKinematicTarget(transform);
		}
	}

}

