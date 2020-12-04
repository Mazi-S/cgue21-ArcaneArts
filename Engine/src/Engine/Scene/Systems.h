#pragma once

#include <glm/glm.hpp>
#include <entt.hpp>

#include "Engine/Core/Base.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Renderer/Camera.h"

namespace Engine::System::CharacterController {

	void OnUpdate(entt::registry& registry, Timestep ts);

}

namespace Engine::System {


	glm::mat4 GetTransform(entt::registry& registry, const entt::entity& entity);

}

namespace Engine::System::Camera {

	Engine::Camera GetCamera(entt::registry& registry);

	void SetViewportSize(entt::registry& registry, uint32_t width, uint32_t height);

}