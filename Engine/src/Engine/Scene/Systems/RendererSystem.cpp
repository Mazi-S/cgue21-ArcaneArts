#include "egpch.h"

#include "Util.h"

#include "Engine/Scene/Components.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/ShadowMap.h"
#include "RendererSystem.h"

namespace Engine::System::Renderer {

	void Submit(entt::registry& registry)
	{

		// objects with shadows
		{
			auto view = registry.view<Component::Core::TransformComponent, Component::Renderer::MaterialComponent, Component::Renderer::MeshComponent, Component::Renderer::ShadowComponent>();
			for (auto entity : view)
			{
				auto [material, mesh] = view.get<Component::Renderer::MaterialComponent, Component::Renderer::MeshComponent>(entity);
				glm::mat4 transform = Util::GlobalTransformMatrix(registry, entity);
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
				Engine::Renderer::Submit(mesh, material, Util::GlobalTransformMatrix(registry, entity));
			}
		}
	}

	void OnUpdateParticleSystem(entt::registry& registry, Timestep ts)
	{
		auto view = registry.view<Component::Renderer::ParticleSystemComponent>();
		for (auto entity : view)
		{
			auto& psComp = view.get<Component::Renderer::ParticleSystemComponent>(entity);
			if (psComp.ParticleSystem != nullptr)
			{
				psComp.ParticleSystem->OnUpdate(ts, Util::Position(registry, entity), psComp.EmitPower, psComp.Cooling, psComp.ParticleSize, psComp.ColorStart, psComp.ColorEnd);
			}
		}
	}

	void OnRenderParticleSystem(entt::registry& registry, const glm::mat4& viewProjection, const glm::vec3& cameraPos)
	{
		auto view = registry.view<Component::Renderer::ParticleSystemComponent>();
		for (auto entity : view)
		{
			auto& particleSystemComp = view.get<Component::Renderer::ParticleSystemComponent>(entity);
			if (particleSystemComp.ParticleSystem != nullptr)
			{
				particleSystemComp.ParticleSystem->OnRender(viewProjection, cameraPos);
			}
		}
	}

}