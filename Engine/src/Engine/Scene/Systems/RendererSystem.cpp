#include "egpch.h"

#include "Util.h"

#include "Engine/Scene/Components.h"

#include "Engine/Renderer/Renderer.h"

namespace Engine::System::Renderer {

	void Submit(entt::registry& registry)
	{

		// objects with shadows
		{
			auto view = registry.view<TransformComponent, MaterialComponent, MeshComponent, ShadowComponent>();
			for (auto entity : view)
			{
				auto [material, mesh] = view.get<MaterialComponent, MeshComponent>(entity);
				Engine::Renderer::Submit(mesh, material, Util::Transform(registry, entity), true);
			}
		}

		// objects without shadows
		{
			auto view = registry.view<TransformComponent, MaterialComponent, MeshComponent>(entt::exclude<ShadowComponent>);
			for (auto entity : view)
			{
				auto [material, mesh] = view.get<MaterialComponent, MeshComponent>(entity);
				Engine::Renderer::Submit(mesh, material, Util::Transform(registry, entity), false);
			}
		}
	}

}