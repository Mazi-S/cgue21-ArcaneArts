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

	void RecalculateProjection(CameraComponent& cc)
	{
		cc.Projection = glm::perspective(cc.FOV, cc.AspectRatio, cc.Near, cc.Far);
	}

}

