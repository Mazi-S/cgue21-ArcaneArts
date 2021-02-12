#include "egpch.h"
#include "PhysicsSystem.h"
#include "Engine/Scene/Components.h"
#include "Engine/Util/Math.h"

#include "PxPhysicsAPI.h"

namespace Engine::System::Physics {

	void OnUpdate(entt::registry& registry)
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

}

