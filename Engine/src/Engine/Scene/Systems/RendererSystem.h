#pragma once
#include <entt.hpp>
#include <glm/glm.hpp>

#include "Engine/Core/Timestep.h"

namespace Engine::System::Renderer {

	void Submit(entt::registry& registry);

	void OnUpdateParticleSystem(entt::registry& registry, Timestep ts);
	void OnRenderParticleSystem(entt::registry& registry, const glm::mat4& viewProjection, const glm::vec3& cameraPos);
}
