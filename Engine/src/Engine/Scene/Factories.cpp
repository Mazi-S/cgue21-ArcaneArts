#include "egpch.h"
#include "Factories.h"
#include "Systems.h"
#include "Components.h"

#include "Engine/Core/Identifier.h"

namespace Engine::Factory {

	entt::entity CreateEntity(entt::registry& registry, const std::string& name)
	{
		entt::entity entity = registry.create();
		registry.emplace<Component::Core::Identifier>(entity, Identifier::newID());
		registry.emplace<Component::Core::TagComponent>(entity, name.empty() ? "Entity" : name);
		registry.emplace<Component::Core::TransformComponent>(entity);
		return entity;
	}

	entt::entity CreateEntity(entt::registry& registry, const std::string& name, uint32_t id)
	{
		entt::entity entity = registry.create();
		registry.emplace<Component::Core::Identifier>(entity, Identifier::use(id));
		registry.emplace<Component::Core::TagComponent>(entity, name.empty() ? "Entity" : name);
		registry.emplace<Component::Core::TransformComponent>(entity);
		return entity;
	}

}