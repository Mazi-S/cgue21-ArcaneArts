#include "egpch.h"
#include "LightSystem.h"

#include "Engine/Scene/Components.h"
#include "Util.h"

namespace Engine::System::Light {

	DirectionalLight GetDirectionalLight(entt::registry& registry)
	{
		auto view = registry.view<DirectionalLightComponent, TransformComponent>();
		for (const entt::entity e : view)
		{
			auto& dlc = view.get<DirectionalLightComponent>(e);
			return DirectionalLight(dlc.Direction, dlc.Color);
		}
		return DirectionalLight();
	}

	std::vector<PointLight> GetPointLights(entt::registry& registry)
	{
		std::vector<PointLight> lights;
		auto view = registry.view<PointLightComponent, TransformComponent>();
		for (const entt::entity e : view)
		{
			auto& plc = view.get<PointLightComponent>(e);
			lights.push_back(PointLight(Util::Position(registry, e), plc.Color, plc.Constant, plc.Linear, plc.Quadratic));
		}
		return lights;
	}

}