#include "egpch.h"
#include "Physics.h"

#include "PxPhysicsAPI.h"

physx::PxDefaultAllocator s_AllocatorCallback;
physx::PxDefaultErrorCallback s_ErrorCallback;

physx::PxFoundation* s_Foundation;
physx::PxPhysics*  s_physicsSDK;


namespace Engine {

	void Physics::Init()
	{
		s_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, s_AllocatorCallback, s_ErrorCallback);

		ASSERT(s_Foundation, "PxCreateFoundation failed!");

		s_physicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *s_Foundation, physx::PxTolerancesScale());

		ASSERT(s_physicsSDK, "PxCreatePhysics failed!");

		// cooking...
	}

	void Physics::Shutdown()
	{
		s_physicsSDK->release();
		s_Foundation->release();
	}

}