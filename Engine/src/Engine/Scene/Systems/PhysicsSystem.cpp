#include "egpch.h"
#include "PhysicsSystem.h"
#include "Engine/Scene/Components.h"

namespace Engine::System::Physics {

	void OnUpdate(entt::registry& registry, Timestep ts)
	{
		auto view = registry.view<VelocityComponent, TransformComponent>();
		for (const entt::entity e : view)
		{
			auto& [vc, tc] = view.get<VelocityComponent, TransformComponent>(e);
			// todo: simulate physics
			tc.Translation += vc.Velocity * float(ts);
		}
	}

}

