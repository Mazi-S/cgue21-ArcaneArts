#include "egpch.h"
#include "Factories.h"
#include "Systems.h"
#include "Components.h"

#include "Engine/Core/Identifier.h"

namespace Engine::Factory {

	entt::entity CreateEntity(entt::registry& registry, const std::string& name)
	{
		entt::entity entity = registry.create();
		registry.emplace<Component::Core::Identifier>(entity, Identifier::newID());
		registry.emplace<Component::Core::TagComponent>(entity, name.empty() ? "Entity" : name);
		registry.emplace<Component::Core::TransformComponent>(entity);
		return entity;
	}

	entt::entity CreateEntity(entt::registry& registry, const std::string& name, uint32_t id)
	{
		entt::entity entity = registry.create();
		registry.emplace<Component::Core::Identifier>(entity, Identifier::use(id));
		registry.emplace<Component::Core::TagComponent>(entity, name.empty() ? "Entity" : name);
		registry.emplace<Component::Core::TransformComponent>(entity);
		return entity;
	}

	entt::entity Copy(entt::registry& registry, entt::entity entity)
	{
		using TagComponent			= Engine::Component::Core::TagComponent;
		using Identifier			= Engine::Component::Core::Identifier;
		using Unserializable		= Engine::Component::Core::Unserializable;
		using TransformComponent	= Engine::Component::Core::TransformComponent;
		using ParentComponent		= Engine::Component::Core::ParentComponent;

		using MeshComponent					= Engine::Component::Renderer::MeshComponent;
		using MaterialComponent				= Engine::Component::Renderer::MaterialComponent;
		using ShadowComponent				= Engine::Component::Renderer::ShadowComponent;
		using DirectionalLightComponent		= Engine::Component::Renderer::DirectionalLightComponent;
		using PointLightComponent			= Engine::Component::Renderer::PointLightComponent;
		using CameraComponent				= Engine::Component::Renderer::CameraComponent;

		using StaticColliderComponent		= Engine::Component::Physics::StaticColliderComponent;
		using CharacterControllerComponent	= Engine::Component::Physics::CharacterControllerComponent;

		using Sound2DComponent		= Engine::Component::Audio::Sound2DComponent;
		using Sound3DComponent		= Engine::Component::Audio::Sound3DComponent;
		using ListenerComponent		= Engine::Component::Audio::ListenerComponent;

		std::string name = registry.get<TagComponent>(entity).Tag;

		entt::entity copiedEntity = CreateEntity(registry, name);

		if (registry.has<ParentComponent>(entity))
			registry.emplace<ParentComponent>(copiedEntity, registry.get<ParentComponent>(entity));

		registry.replace<TransformComponent>(copiedEntity, registry.get<TransformComponent>(entity));

		if (registry.has<Unserializable>(entity))
			registry.emplace<Unserializable>(copiedEntity);

		// Renderer
		if (registry.has<MeshComponent>(entity))
			registry.emplace<MeshComponent>(copiedEntity, registry.get<MeshComponent>(entity));

		if (registry.has<MaterialComponent>(entity))
			registry.emplace<MaterialComponent>(copiedEntity, registry.get<MaterialComponent>(entity));

		if (registry.has<ShadowComponent>(entity))
			registry.emplace<ShadowComponent>(copiedEntity);

		if (registry.has<DirectionalLightComponent>(entity))
			registry.emplace<DirectionalLightComponent>(copiedEntity, registry.get<DirectionalLightComponent>(entity));

		if (registry.has<PointLightComponent>(entity))
			registry.emplace<PointLightComponent>(copiedEntity, registry.get<PointLightComponent>(entity));

		if (registry.has<CameraComponent>(entity))
			registry.emplace<CameraComponent>(copiedEntity, registry.get<CameraComponent>(entity));

		// Physics
		if (registry.has<StaticColliderComponent>(entity))
			registry.emplace<StaticColliderComponent>(copiedEntity);

		if (registry.has<CharacterControllerComponent>(entity))
			registry.emplace<CharacterControllerComponent>(copiedEntity, registry.get<CharacterControllerComponent>(entity));

		// Audio
		if (registry.has<Sound2DComponent>(entity))
			registry.emplace<Sound2DComponent>(copiedEntity, registry.get<Sound2DComponent>(entity));

		if (registry.has<Sound3DComponent>(entity))
			registry.emplace<Sound3DComponent>(copiedEntity, registry.get<Sound3DComponent>(entity));

		if (registry.has<ListenerComponent>(entity))
			registry.emplace<ListenerComponent>(copiedEntity);

		return copiedEntity;
	}

}