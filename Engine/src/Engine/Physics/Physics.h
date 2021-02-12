#pragma once

#include "PxPhysicsAPI.h"
#include "Engine/Renderer/Mesh.h"
#include "glm/glm.hpp"

namespace Engine {

	class Physics
	{
	public:
		static void Init();
		static void Shutdown();

		static physx::PxScene* CreateScene();

		static physx::PxRigidDynamic* CreateRegidDynamicSphere(glm::vec3 position, float radius);
		static physx::PxRigidDynamic* CreateRegidDynamic();
		static physx::PxRigidStatic* CreateRigidStatic(Ref<Mesh> mesh, glm::vec3 position);
	private:

	};

}