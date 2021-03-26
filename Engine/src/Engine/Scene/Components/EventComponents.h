#pragma once
#include <entt.hpp>

namespace Engine::Component::Event {

	struct CollisionEventComponent
	{
		entt::entity Other;

		CollisionEventComponent(const CollisionEventComponent&) = default;
		CollisionEventComponent(entt::entity other)
			: Other(other) { }
	};

}