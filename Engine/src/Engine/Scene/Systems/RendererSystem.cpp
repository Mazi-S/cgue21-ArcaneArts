#include "egpch.h"

#include "Util.h"

#include "Engine/Scene/Components.h"

#include "Engine/Renderer/Renderer.h"

namespace Engine::System::Renderer {

	void Submit(entt::registry& registry)
	{
		auto view = registry.view<TransformComponent, MaterialComponent, MeshComponent>();
		for (auto entity : view)
		{
			auto [material, mesh] = view.get<MaterialComponent, MeshComponent>(entity);
			Engine::Renderer::Submit(mesh, material, Util::Transform(registry, entity));
		}
	}

}