#include "egpch.h"
#include "Factories.h"
#include "Systems.h"
#include "Components.h"

namespace Engine::Factory {

	entt::entity CreateEntity(entt::registry& registry, const std::string& name)
	{
		entt::entity entity = registry.create();
		registry.emplace<TransformComponent>(entity);
		registry.emplace<TagComponent>(entity, name.empty() ? "Entity" : name);
		return entity;
	}

}