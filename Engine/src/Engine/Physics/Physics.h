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

		// todo: remove or fix (just for testing)
		static physx::PxRigidDynamic* CreateRegidDynamic();

		static physx::PxController* CreateController(physx::PxControllerManager* manager, float height = 1.0f, float radius = 0.5f, glm::vec3 position = { 0.0f, 0.0f, 0.0f });

		// todo: kinemetic object (instead of dynamic); prevents tunneling 
		static physx::PxRigidDynamic* CreateRegidDynamicSphere(glm::vec3 position, float radius);
		static physx::PxRigidStatic* CreateRigidStatic(Ref<Mesh> mesh, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	private:

	};

}