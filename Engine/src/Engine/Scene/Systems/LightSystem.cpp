#include "egpch.h"
#include "LightSystem.h"

#include "Engine/Scene/Components.h"
#include "Util.h"

namespace Engine::System::Light {

	DirectionalLight GetDirectionalLight(entt::registry& registry)
	{
		auto view = registry.view<Component::Renderer::DirectionalLightComponent, Component::Core::TransformComponent>();
		for (const entt::entity e : view)
		{
			auto& dlc = view.get<Component::Renderer::DirectionalLightComponent>(e);
			return DirectionalLight(dlc.Direction, dlc.Color);
		}
		return DirectionalLight();
	}

	std::vector<PointLight> GetPointLights(entt::registry& registry)
	{
		std::vector<PointLight> lights;
		auto view = registry.view<Component::Renderer::PointLightComponent, Component::Core::TransformComponent>();
		for (const entt::entity e : view)
		{
			auto& plc = view.get<Component::Renderer::PointLightComponent>(e);
			lights.push_back(PointLight(Util::Position(registry, e), plc.Color, plc.Constant, plc.Linear, plc.Quadratic));
		}
		return lights;
	}

}