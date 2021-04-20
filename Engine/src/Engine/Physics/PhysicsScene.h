#pragma once

#include "PxPhysicsAPI.h"
#include "Engine/Events/Event.h"
#include "Engine/Scene/Entity.h"

#include <map>
#include <glm/glm.hpp>

namespace Engine::Physics {

	class PsScene : public physx::PxSimulationEventCallback
	{
		using EventCallbackFn = std::function<void(Event&)>;

	public:
		PsScene();
		~PsScene();

		void Simulate(float elapsedTime);

		void AddActor(physx::PxActor* actor, Entity entity);
		void RemoveActor(physx::PxActor* actor);

		void SetEventCallback(EventCallbackFn callback);

		physx::PxController* CreateController(float height, float radius, glm::vec3 position);

	private:
		physx::PxScene* m_PxScene;
		physx::PxControllerManager* m_PxControllerManager;
		
		// TODO: combine
		std::map<physx::PxActor*, Entity> m_Entities;

		// Callback Function
		EventCallbackFn m_EventCallback;


		// Inherited via PxSimulationEventCallback
		virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override {}
		virtual void onWake(physx::PxActor** actors, physx::PxU32 count) override {}
		virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) override {}
		virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override {}
		virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
		virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override {}
	};

}