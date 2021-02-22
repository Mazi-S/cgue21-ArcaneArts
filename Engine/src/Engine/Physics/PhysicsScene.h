#pragma once

#include "PxPhysicsAPI.h"

#include <glm/glm.hpp>

namespace Engine::Physics {

	class PsScene
	{
	public:
		PsScene();

		void Simulate(float elapsedTime);

		void AddActor(physx::PxActor* actor);
		physx::PxController* CreateController(float height, float radius, glm::vec3 position);

	private:
		physx::PxScene* m_PxScene;
		physx::PxControllerManager* m_PxControllerManager;

	};

}