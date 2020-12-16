#pragma once

#include <entt.hpp>
#include "Engine/Renderer/Light.h"

namespace Engine::System::Light {

	 DirectionalLight GetDirectionalLight(entt::registry& registry);
	 std::vector<PointLight> GetPointLights(entt::registry& registry);
}