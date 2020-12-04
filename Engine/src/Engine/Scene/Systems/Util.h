#pragma once

#include <glm/glm.hpp>

#include "Engine/Scene/Components.h"

namespace Engine::System::Util {

	glm::mat4 Transform(const TransformComponent& tc);

	void RecalculateProjection(CameraComponent& cc);
}