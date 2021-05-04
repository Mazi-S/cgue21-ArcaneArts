#include "egpch.h"

#include "Util.h"

#include "Engine/Scene/Components.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/ShadowMap.h"

namespace Engine::System::Renderer {

	void Submit(entt::registry& registry)
	{

		// objects with shadows
		{
			auto view = registry.view<Component::Core::TransformComponent, Component::Renderer::MaterialComponent, Component::Renderer::MeshComponent, Component::Renderer::ShadowComponent>();
			for (auto entity : view)
			{
				auto [material, mesh] = view.get<Component::Renderer::MaterialComponent, Component::Renderer::MeshComponent>(entity);
				glm::mat4 transform = Util::Transform(registry, entity);
				Engine::Renderer::Submit(mesh, material, transform);
				Engine::ShadowMap::Submit(mesh, transform);
			}
		}

		// objects without shadows
		{
			auto view = registry.view<Component::Core::TransformComponent, Component::Renderer::MaterialComponent, Component::Renderer::MeshComponent>(entt::exclude<Component::Renderer::ShadowComponent>);
			for (auto entity : view)
			{
				auto [material, mesh] = view.get<Component::Renderer::MaterialComponent, Component::Renderer::MeshComponent>(entity);
				Engine::Renderer::Submit(mesh, material, Util::Transform(registry, entity));
			}
		}
	}

}