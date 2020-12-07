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


	entt::entity CreateCamera(entt::registry& registry, entt::entity parent, glm::vec3 offset, glm::vec3 rotation)
	{
		entt::entity camera = registry.create();
		auto& tc = registry.emplace<TransformComponent>(camera);
		tc.Translation = offset; tc.Rotation = rotation;
		registry.emplace<TagComponent>(camera, "Camera");
		registry.emplace<CameraComponent>(camera);
		if(parent != entt::null)
			registry.emplace<ParentComponent>(camera, parent);

		return camera;
	}

}