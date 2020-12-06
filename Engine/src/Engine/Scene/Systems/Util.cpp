#include "egpch.h"
#include "Util.h"

#include <glm/glm.hpp>

namespace Engine::System::Util {

	glm::mat4 Transform(const TransformComponent& tc)
	{
		return glm::translate(glm::mat4(1.0f), tc.Translation)
			* glm::toMat4(glm::quat(tc.Rotation))
			* glm::scale(glm::mat4(1.0f), tc.Scale);
	}

	glm::mat4 Transform(entt::registry& registry, entt::entity entity)
	{
		glm::mat4 transform = glm::mat4(1.0f);

		auto* tc = registry.try_get<TransformComponent>(entity);
		if (tc != nullptr)
			transform = Util::Transform(*tc);

		auto* pc = registry.try_get<ParentComponent>(entity);
		if (pc != nullptr)
			transform = Transform(registry, pc->Parent) * transform;

		return transform;
	}

	TransformComponent Combine(const TransformComponent& parent, const TransformComponent& child)
	{
		TransformComponent tc(child);
		tc.Scale *= parent.Scale;
		tc.Rotation += parent.Rotation;
		
		glm::vec4 translation = glm::toMat4(glm::quat(tc.Rotation)) * glm::vec4{ parent.Translation.x, parent.Translation.y, parent.Translation.z, 1.0f};
		tc.Translation.x += translation.x;
		tc.Translation.y += translation.y;
		tc.Translation.z += translation.z;

		return tc;
	}

	void RecalculateProjection(CameraComponent& cc)
	{
		cc.Projection = glm::perspective(cc.FOV, cc.AspectRatio, cc.Near, cc.Far);
	}

}

