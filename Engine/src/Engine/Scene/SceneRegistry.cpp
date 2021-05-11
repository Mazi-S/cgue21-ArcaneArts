#include "egpch.h"
#include "Scene.h"

#include "Engine/Core/Application.h"
#include "Engine/Util/Math.h"

#include "Components.h"
#include "Systems.h"
#include "Factories.h"
#include "Entity.h"

// Renderer
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/ShadowMap.h"
#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/Light.h"

#include "Engine/Renderer/MeshLibrary.h"

// Audio
#include "Engine/Audio/SoundEngine.h"

namespace Engine {

	static void UnbindScript(entt::registry& registry, entt::entity entity)
	{
		registry.get<Component::Core::NativeScriptComponent>(entity).Unbind();
	}

	void Scene::InitCameraComponent(entt::registry& registry, entt::entity entity)
	{
		System::Camera::SetViewportSize(registry, entity, m_ViewportWidth, m_ViewportHeight);
	}

	void Scene::InitStaticCollider(entt::registry& registry, entt::entity entity)
	{
		auto [transformComp, meshComp, staticColliderComp] = registry.try_get<Component::Core::TransformComponent, Component::Renderer::MeshComponent, Component::Physics::StaticColliderComponent>(entity);
		if (transformComp != nullptr && meshComp != nullptr && staticColliderComp != nullptr)
		{
			auto actor = Engine::PhysicsAPI::CreateRigidStatic(transformComp->Translation, transformComp->Rotation);

			Ref<Physics::PsMesh> pxMesh = MeshLibrary::Get(meshComp->Mesh)->GetPsMesh();
			physx::PxTriangleMesh* triMesh = pxMesh->GetPxTriangleMesh();
			auto shape = Engine::PhysicsAPI::CreateShape(triMesh, transformComp->Scale);

			actor->attachShape(*shape);
			shape->release();
			staticColliderComp->Actor = actor;

			m_PhysicsScene->AddActor(actor, Entity(entity, &registry));
		}
	}

	void Scene::UpdateStaticCollider(entt::registry& registry, entt::entity entity)
	{
		DestroyStaticCollider(registry, entity);
		InitStaticCollider(registry, entity);
	}

	void Scene::DestroyStaticCollider(entt::registry& registry, entt::entity entity)
	{
		auto staticColliderComp = registry.try_get<Component::Physics::StaticColliderComponent>(entity);

		if (staticColliderComp != nullptr && staticColliderComp->Actor != nullptr)
		{
			m_PhysicsScene->RemoveActor(staticColliderComp->Actor);
			staticColliderComp->Actor->release();
			staticColliderComp->Actor = nullptr;
		}
	}

	void Scene::UpdateTransform(entt::registry& registry, entt::entity entity)
	{
		// TODO: recalculate matrix...
	}

	void Scene::InitCharacterController(entt::registry& registry, entt::entity entity)
	{
		auto [transformComp, characterControllerComp] = registry.try_get<Component::Core::TransformComponent, Component::Physics::CharacterControllerComponent>(entity);
		characterControllerComp->Controller = m_PhysicsScene->CreateController(characterControllerComp->StandingHeight - 2.0f * characterControllerComp->Radius, characterControllerComp->Radius, transformComp->Translation);
	}

	void Scene::UpdateCharacterController(entt::registry& registry, entt::entity entity)
	{
		auto [transformComp, characterControllerComp] = registry.try_get<Component::Core::TransformComponent, Component::Physics::CharacterControllerComponent>(entity);

		if (characterControllerComp->Controller != nullptr)
		{
			characterControllerComp->Controller->release();
			characterControllerComp->Controller == nullptr;
		}

		characterControllerComp->Controller = m_PhysicsScene->CreateController(characterControllerComp->StandingHeight - 2.0f * characterControllerComp->Radius, characterControllerComp->Radius, transformComp->Translation);
	}

	void Scene::DestroyCharacterController(entt::registry& registry, entt::entity entity)
	{
		auto& characterControllerComp = registry.get<Component::Physics::CharacterControllerComponent>(entity);

		if (characterControllerComp.Controller != nullptr)
		{
			characterControllerComp.Controller->release();
			characterControllerComp.Controller == nullptr;
		}
	}

	void Scene::AddRigidComponent(entt::registry& registry, entt::entity entity)
	{
		auto& rc = registry.get<Component::Physics::RigidComponent>(entity);
		m_PhysicsScene->AddActor(rc.Actor, Entity(entity, &registry));
	}

	void Scene::RemoveRigidComponent(entt::registry& registry, entt::entity entity)
	{
		auto& rc = registry.get<Component::Physics::RigidComponent>(entity);
		m_PhysicsScene->RemoveActor(rc.Actor);
	}

	void Scene::AddRigidDynamicComponent(entt::registry& registry, entt::entity entity)
	{
		auto& rdc = registry.get<Component::Physics::RigidDynamicComponent>(entity);
		m_PhysicsScene->AddActor(rdc.Actor, Entity(entity, &registry));
	}

	void Scene::RemoveRigidDynamicComponent(entt::registry& registry, entt::entity entity)
	{
		auto& rdc = registry.get<Component::Physics::RigidDynamicComponent>(entity);
		m_PhysicsScene->RemoveActor(rdc.Actor);
	}


	void Scene::AddSound2DComponent(entt::registry& registry, entt::entity entity)
	{
		auto& sound2DComp = registry.get<Component::Audio::Sound2DComponent>(entity);
		sound2DComp.Sound = Engine::SoundEngine::Play2D(sound2DComp.SoundSource, sound2DComp.Loop, false, true);
	}

	void Scene::RemoveSound2DComponent(entt::registry& registry, entt::entity entity)
	{
		auto& sound2DComp = registry.get<Component::Audio::Sound2DComponent>(entity);
		sound2DComp.Sound->stop();
		sound2DComp.Sound->drop();
	}

	void Scene::AddSound3DComponent(entt::registry& registry, entt::entity entity)
	{
		auto& [sound3DComp, transformComp] = registry.get<Component::Audio::Sound3DComponent, Component::Core::TransformComponent>(entity);
		sound3DComp.Sound = Engine::SoundEngine::Play3D(sound3DComp.SoundSource, transformComp.Translation, sound3DComp.Loop, false, true);
	}

	void Scene::RemoveSound3DComponent(entt::registry& registry, entt::entity entity)
	{
		auto& sound3DComp = registry.get<Component::Audio::Sound3DComponent>(entity);
		sound3DComp.Sound->stop();
		sound3DComp.Sound->drop();
	}

	void Scene::InitRegistry()
	{
		// Core //////////////////////////////////////////////////////////////////////////////////////////
		m_Registry.on_destroy<Component::Core::NativeScriptComponent>().connect<&UnbindScript>();
		m_Registry.on_update<Component::Core::TransformComponent>().connect<&Scene::UpdateTransform>(*this);


		// Renderer components ///////////////////////////////////////////////////////////////////////////
		m_Registry.on_construct<Component::Renderer::CameraComponent>().connect<&Scene::InitCameraComponent>(*this);


		// Audio //////////////////////7//////////////////////////////////////////////////////////////////
		m_Registry.on_construct<Component::Audio::Sound2DComponent>().connect<&Scene::AddSound2DComponent>(*this);
		m_Registry.on_destroy<Component::Audio::Sound2DComponent>().connect<&Scene::RemoveSound2DComponent>(*this);

		m_Registry.on_construct<Component::Audio::Sound3DComponent>().connect<&Scene::AddSound3DComponent>(*this);
		m_Registry.on_destroy<Component::Audio::Sound3DComponent>().connect<&Scene::RemoveSound3DComponent>(*this);


		// Physics components ////////////////////////////////////////////////////////////////////////////
		// Static Collider
		m_Registry.on_construct<Component::Physics::StaticColliderComponent>().connect<&Scene::InitStaticCollider>(*this);
		m_Registry.on_update<Component::Physics::StaticColliderComponent>().connect<&Scene::UpdateStaticCollider>(*this);
		m_Registry.on_destroy<Component::Physics::StaticColliderComponent>().connect<&Scene::DestroyStaticCollider>(*this);

		// Character Controller
		m_Registry.on_construct<Component::Physics::CharacterControllerComponent>().connect<&Scene::InitCharacterController>(*this);
		m_Registry.on_update<Component::Physics::CharacterControllerComponent>().connect<&Scene::UpdateCharacterController>(*this);
		m_Registry.on_destroy<Component::Physics::CharacterControllerComponent>().connect<&Scene::DestroyCharacterController>(*this);

		// Rigid
		m_Registry.on_construct<Component::Physics::RigidComponent>().connect<&Scene::AddRigidComponent>(*this);
		m_Registry.on_destroy<Component::Physics::RigidComponent>().connect<&Scene::RemoveRigidComponent>(*this);

		// Rigid Dynamic
		m_Registry.on_construct<Component::Physics::RigidDynamicComponent>().connect<&Scene::AddRigidDynamicComponent>(*this);
		m_Registry.on_destroy<Component::Physics::RigidDynamicComponent>().connect<&Scene::RemoveRigidDynamicComponent>(*this);
	}
}