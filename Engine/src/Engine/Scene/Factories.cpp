#include "egpch.h"
#include "Factories.h"
#include "Systems.h"
#include "Components.h"

namespace Engine::Factory {

	entt::entity CreateEntity(entt::registry& registry, const std::string& name)
	{
		entt::entity entity = registry.create();
		registry.emplace<Component::Core::TransformComponent>(entity);
		registry.emplace<Component::Core::TagComponent>(entity, name.empty() ? "Entity" : name);
		return entity;
	}

}