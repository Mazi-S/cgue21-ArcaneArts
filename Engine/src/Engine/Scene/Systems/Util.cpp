#include "egpch.h"
#include "Util.h"

#include <glm/glm.hpp>
#include "Engine/Core/Input.h"

namespace Engine::System::Util {

	void MakeIndependent(entt::registry& registry, entt::entity entity)
	{
		registry.replace<Component::Core::TransformComponent>(entity, GlobalTransform(registry, entity));
		registry.remove<Engine::Component::Core::ParentComponent>(entity);
	}

	Component::Core::TransformComponent GlobalTransform(entt::registry& registry, entt::entity entity)
	{
		Component::Core::TransformComponent transformComp = registry.get<Component::Core::TransformComponent>(entity);

		auto* parentComp = registry.try_get<Component::Core::ParentComponent>(entity);

		if (parentComp == nullptr || parentComp->Parent == entt::null)
			return transformComp;

		const auto& transformComp_parent = GlobalTransform(registry, parentComp->Parent);
		transformComp.Rotation += transformComp_parent.Rotation;
		transformComp.Scale *= transformComp_parent.Scale;
		transformComp.Translation = Engine::System::Util::Transform(transformComp_parent, transformComp.Translation);

		return transformComp;
	}

	glm::mat4 Transform(const Component::Core::TransformComponent& tc)
	{
		return glm::translate(glm::mat4(1.0f), tc.Translation)
			* glm::toMat4(glm::quat(tc.Rotation))
			* glm::scale(glm::mat4(1.0f), tc.Scale);
	}

	glm::vec3 Transform(const Component::Core::TransformComponent& tc, glm::vec3 v3)
	{
		glm::vec4 v4 = Transform(tc) * glm::vec4{ v3.x, v3.y, v3.z, 1.0f};
		return { v4.x, v4.y, v4.z };
	}

	glm::mat4 GlobalTransformMatrix(entt::registry& registry, entt::entity entity)
	{
		glm::mat4 transform = glm::mat4(1.0f);

		auto* tc = registry.try_get<Component::Core::TransformComponent>(entity);
		if (tc != nullptr)
			transform = Util::Transform(*tc);

		auto* pc = registry.try_get<Component::Core::ParentComponent>(entity);
		if (pc != nullptr && pc->Parent != entt::null)
			transform = GlobalTransformMatrix(registry, pc->Parent) * transform;

		return transform;
	}

	glm::vec3 Position(entt::registry& registry, entt::entity entity)
	{
		glm::mat4 transform = GlobalTransformMatrix(registry, entity);
		return { transform[3][0], transform[3][1], transform[3][2] };
	}

	void RecalculateProjection(Component::Renderer::CameraComponent& cc)
	{
		cc.Projection = glm::perspective(cc.FOV, cc.AspectRatio, cc.Near, cc.Far);
	}

}

