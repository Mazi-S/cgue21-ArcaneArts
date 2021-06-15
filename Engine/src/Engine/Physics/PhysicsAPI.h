#pragma once

#include "PxPhysicsAPI.h"
#include "Engine/Renderer/Mesh.h"
#include "glm/glm.hpp"

namespace Engine {

	class PhysicsAPI
	{
	public:
		static void Init();
		static void Shutdown();

		static physx::PxScene* CreateScene();

		static physx::PxController* CreateController(physx::PxControllerManager* manager, float height = 1.0f, float radius = 0.5f, glm::vec3 position = { 0.0f, 0.0f, 0.0f });

		static physx::PxRigidDynamic* CreateRigidDynamic(glm::vec3 position, glm::vec3 rotation = {0.0f, 0.0f, 0.0f});
		static physx::PxRigidStatic* CreateRigidStatic(glm::vec3 position, glm::vec3 rotation = { 0.0f, 0.0f, 0.0f });

		static physx::PxShape* CreateSphereShape(float radius, physx::PxMaterial* material);
		static physx::PxShape* CreateShape(physx::PxTriangleMesh* mesh, glm::vec3 scale, physx::PxMaterial* material);
		static physx::PxShape* CreateShape(physx::PxConvexMesh* mesh, glm::vec3 scale, physx::PxMaterial* material);

		static physx::PxMaterial* CreateMaterial(float staticFriction, float dynamicFriction, float restitution);

		static physx::PxTriangleMesh* CreateTriangleMesh(Physics::PsMesh* mesh);
		static physx::PxConvexMesh* CreateConvexMesh(Physics::PsMesh* mesh);

		static void SetKinematic(physx::PxRigidDynamic* actor, bool kinematic);
		static void SetTrigger(physx::PxShape* shape, bool trigger);
		static void SetSimulation(physx::PxShape* shape, bool simulation);
	private:
	};
}