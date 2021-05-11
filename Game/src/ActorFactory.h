#pragma once
#include "Engine.h"

#include <PxPhysicsAPI.h>
#include <glm/glm.hpp>

namespace ActorFactory {

	physx::PxRigidDynamic* Monster(std::string meshName, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	physx::PxRigidDynamic* MagicBall(glm::vec3 position, glm::vec3 rotation, float radius);

}