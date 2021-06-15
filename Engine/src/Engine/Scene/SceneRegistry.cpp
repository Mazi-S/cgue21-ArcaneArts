#include "egpch.h"
#include "Scene.h"

#include "Engine/Core/Application.h"
#include "Engine/Util/Math.h"

#include "Components.h"
#include "Systems.h"
#include "Factories.h"
#include "Entity.h"

#include "Engine/Physics/PhysicsAPI.h"

// Renderer
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/ShadowMap.h"
#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/Light.h"

#include "Engine/Renderer/MeshLibrary.h"

// Audio
#include "Engine/Audio/SoundLibrary.h"

namespace Engine {

	static void UnbindScript(entt::registry& registry, entt::entity entity)
	{
		registry.get<Component::Core::NativeScriptComponent>(entity).Unbind();
	}

	void Scene::InitCameraComponent(entt::registry& registry, entt::entity entity)
	{
		System::Camera::SetViewportSize(registry, entity, m_ViewportWidth, m_ViewportHeight);
	}

	void Scene::DestroyCameraComponent(entt::registry& registry, entt::entity entity)
	{
		if (m_MainCamera == entity)
			m_MainCamera = entt::null;
	}

	void Scene::InitParticleSystem(entt::registry& registry, entt::entity entity)
	{
		auto& psComp = registry.get<Component::Renderer::ParticleSystemComponent>(entity);

		glm::vec3 position = System::Util::Position(registry, entity);
		psComp.ParticleSystem = new ParticleSystem(psComp.Type, position, psComp.EmitPower, psComp.Cooling, psComp.ParticleSize, psComp.ColorStart, psComp.ColorEnd);
	}

	void Scene::DestroyParticleSystem(entt::registry& registry, entt::entity entity)
	{
		auto& particleSystemComp = registry.get<Component::Renderer::ParticleSystemComponent>(entity);
		delete particleSystemComp.ParticleSystem;
		particleSystemComp.ParticleSystem = nullptr;
	}

	void Scene::InitPhysicsMaterial(entt::registry& registry, entt::entity entity)
	{
		auto& materialComp = registry.get<Component::Physics::PhysicsMaterialComponent>(entity);
		materialComp.Material = PhysicsAPI::CreateMaterial(materialComp.StaticFriction, materialComp.DynamicFriction, materialComp.Restitution);
	}

	void Scene::UpdatePhysicsMaterial(entt::registry& registry, entt::entity entity)
	{
		auto& materialComp = registry.get<Component::Physics::PhysicsMaterialComponent>(entity);
		materialComp.Material->setStaticFriction(materialComp.StaticFriction);
		materialComp.Material->setDynamicFriction(materialComp.DynamicFriction);
		materialComp.Material->setRestitution(materialComp.Restitution);
	}

	void Scene::InitStaticCollider(entt::registry& registry, entt::entity entity)
	{
		auto [meshComp, staticColliderComp] = registry.try_get<Component::Renderer::MeshComponent, Component::Physics::StaticColliderComponent>(entity);

		if (meshComp == nullptr || staticColliderComp == nullptr)
			return;

		if (!registry.has<Component::Physics::PhysicsMaterialComponent>(entity))
			registry.emplace<Component::Physics::PhysicsMaterialComponent>(entity);

		auto& materialComp = registry.get<Component::Physics::PhysicsMaterialComponent>(entity);

		auto transformComp = System::Util::GlobalTransform(registry, entity);
		auto actor = Engine::PhysicsAPI::CreateRigidStatic(transformComp.Translation, transformComp.Rotation);

		Ref<Physics::PsMesh> pxMesh = MeshLibrary::Get(meshComp->Mesh)->GetPsMesh();
		physx::PxTriangleMesh* triMesh = pxMesh->GetPxTriangleMesh();
		auto shape = Engine::PhysicsAPI::CreateShape(triMesh, transformComp.Scale, materialComp.Material);

		actor->attachShape(*shape);
		shape->release();
		staticColliderComp->Actor = actor;

		m_PhysicsScene->AddActor(actor, Entity(entity, &registry));
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

	void Scene::InitDynamicConvex(entt::registry& registry, entt::entity entity)
	{
		auto [meshComp, dynamicConvexComp] = registry.try_get<Component::Renderer::MeshComponent, Component::Physics::DynamicConvexComponent>(entity);

		if (meshComp == nullptr || dynamicConvexComp == nullptr)
			return;

		if (!registry.has<Component::Physics::PhysicsMaterialComponent>(entity))
			registry.emplace<Component::Physics::PhysicsMaterialComponent>(entity);

		auto& materialComp = registry.get<Component::Physics::PhysicsMaterialComponent>(entity);

		auto transformComp = System::Util::GlobalTransform(registry, entity);
		auto actor = Engine::PhysicsAPI::CreateRigidDynamic(transformComp.Translation, transformComp.Rotation);

		Ref<Physics::PsMesh> pxMesh = MeshLibrary::Get(meshComp->Mesh)->GetPsMesh();
		physx::PxConvexMesh* convexMesh = pxMesh->GetPxConvexMesh();

		if (convexMesh == nullptr)
			return;

		auto shape = Engine::PhysicsAPI::CreateShape(convexMesh, transformComp.Scale, materialComp.Material);

		actor->attachShape(*shape);
		shape->release();
		dynamicConvexComp->Actor = actor;

		m_PhysicsScene->AddActor(actor, Entity(entity, &registry));
	}

	void Scene::UpdateDynamicConvex(entt::registry& registry, entt::entity entity)
	{
		DestroyDynamicConvex(registry, entity);
		InitDynamicConvex(registry, entity);
	}

	void Scene::DestroyDynamicConvex(entt::registry& registry, entt::entity entity)
	{
		auto dynamicConvexComp = registry.try_get<Component::Physics::DynamicConvexComponent>(entity);

		if (dynamicConvexComp != nullptr && dynamicConvexComp->Actor != nullptr)
		{
			m_PhysicsScene->RemoveActor(dynamicConvexComp->Actor);
			dynamicConvexComp->Actor->release();
			dynamicConvexComp->Actor = nullptr;
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


	void Scene::InitSound2DComponent(entt::registry& registry, entt::entity entity)
	{
		auto& sound2DComp = registry.get<Component::Audio::Sound2DComponent>(entity);

		if (sound2DComp.SoundSource.empty())
			return;

		Ref<Audio::SoundSource> soundSource = SoundLibrary::Get(sound2DComp.SoundSource);
		sound2DComp.Sound = soundSource->Play2D(sound2DComp.Loop, false, true);
		sound2DComp.Sound->setVolume(sound2DComp.Volume);
	}

	void Scene::UpdateSound2DComponent(entt::registry& registry, entt::entity entity)
	{
		DestroySound2DComponent(registry, entity);
		InitSound2DComponent(registry, entity);
	}

	void Scene::DestroySound2DComponent(entt::registry& registry, entt::entity entity)
	{
		auto& sound2DComp = registry.get<Component::Audio::Sound2DComponent>(entity);

		if (sound2DComp.Sound == nullptr)
			return;

		sound2DComp.Sound->stop();
		sound2DComp.Sound->drop();
		sound2DComp.Sound = nullptr;
	}

	void Scene::InitSound3DComponent(entt::registry& registry, entt::entity entity)
	{
		auto& sound3DComp = registry.get<Component::Audio::Sound3DComponent>(entity);

		if (sound3DComp.SoundSource.empty())
			return;

		Ref<Audio::SoundSource> soundSource = SoundLibrary::Get(sound3DComp.SoundSource);
		glm::vec3 position = System::Util::Position(registry, entity);

		sound3DComp.Sound = soundSource->Play3D(position, sound3DComp.Loop, false, true);
		sound3DComp.Sound->setVolume(sound3DComp.Volume);
	}

	void Scene::UpdateSound3DComponent(entt::registry& registry, entt::entity entity)
	{
		DestroySound3DComponent(registry, entity);
		InitSound3DComponent(registry, entity);
	}

	void Scene::DestroySound3DComponent(entt::registry& registry, entt::entity entity)
	{
		auto& sound3DComp = registry.get<Component::Audio::Sound3DComponent>(entity);

		if (sound3DComp.Sound == nullptr)
			return;

		sound3DComp.Sound->stop();
		sound3DComp.Sound->drop();
		sound3DComp.Sound = nullptr;
	}

	void Scene::InitRegistry()
	{
		// Core //////////////////////////////////////////////////////////////////////////////////////////
		m_Registry.on_destroy<Component::Core::NativeScriptComponent>().connect<&UnbindScript>();
		m_Registry.on_update<Component::Core::TransformComponent>().connect<&Scene::UpdateTransform>(*this);


		// Renderer components ///////////////////////////////////////////////////////////////////////////
		m_Registry.on_construct<Component::Renderer::CameraComponent>().connect<&Scene::InitCameraComponent>(*this);
		m_Registry.on_destroy<Component::Renderer::CameraComponent>().connect<&Scene::DestroyCameraComponent>(*this);

		m_Registry.on_construct<Component::Renderer::ParticleSystemComponent>().connect<&Scene::InitParticleSystem>(*this);
		m_Registry.on_destroy<Component::Renderer::ParticleSystemComponent>().connect<&Scene::DestroyParticleSystem>(*this);


		// Audio //////////////////////7//////////////////////////////////////////////////////////////////
		m_Registry.on_construct<Component::Audio::Sound2DComponent>().connect<&Scene::InitSound2DComponent>(*this);
		m_Registry.on_update<Component::Audio::Sound2DComponent>().connect<&Scene::UpdateSound2DComponent>(*this);
		m_Registry.on_destroy<Component::Audio::Sound2DComponent>().connect<&Scene::DestroySound2DComponent>(*this);

		m_Registry.on_construct<Component::Audio::Sound3DComponent>().connect<&Scene::InitSound3DComponent>(*this);
		m_Registry.on_update<Component::Audio::Sound3DComponent>().connect<&Scene::UpdateSound3DComponent>(*this);
		m_Registry.on_destroy<Component::Audio::Sound3DComponent>().connect<&Scene::DestroySound3DComponent>(*this);


		// Physics components ////////////////////////////////////////////////////////////////////////////
		// Physics Material
		m_Registry.on_construct<Component::Physics::PhysicsMaterialComponent>().connect<&Scene::InitPhysicsMaterial>(*this);
		m_Registry.on_update<Component::Physics::PhysicsMaterialComponent>().connect<&Scene::UpdatePhysicsMaterial>(*this);

		// Static Collider
		m_Registry.on_construct<Component::Physics::StaticColliderComponent>().connect<&Scene::InitStaticCollider>(*this);
		m_Registry.on_update<Component::Physics::StaticColliderComponent>().connect<&Scene::UpdateStaticCollider>(*this);
		m_Registry.on_destroy<Component::Physics::StaticColliderComponent>().connect<&Scene::DestroyStaticCollider>(*this);

		// Dynamic Convex Component
		m_Registry.on_construct<Component::Physics::DynamicConvexComponent>().connect<&Scene::InitDynamicConvex>(*this);
		m_Registry.on_update<Component::Physics::DynamicConvexComponent>().connect<&Scene::UpdateDynamicConvex>(*this);
		m_Registry.on_destroy<Component::Physics::DynamicConvexComponent>().connect<&Scene::DestroyDynamicConvex>(*this);

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