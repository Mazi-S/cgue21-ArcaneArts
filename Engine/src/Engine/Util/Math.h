#pragma once

#include <glm/glm.hpp>
#include <PxPhysicsAPI.h>

namespace Engine::Util {


	class Math
	{
	public:
		
		static glm::vec3 ToEulerAngles(physx::PxQuat q);

		static physx::PxQuat ToQuaternion(glm::vec3 euler);
		
	};

}