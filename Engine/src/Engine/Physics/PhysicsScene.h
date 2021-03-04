#pragma once

#include "PxPhysicsAPI.h"

#include <map>
#include <entt.hpp>
#include <glm/glm.hpp>

namespace Engine::Physics {

	class PsScene : public physx::PxSimulationEventCallback
	{
		using TriggerCallback = std::function<void(entt::entity, entt::entity)>;

	public:
		PsScene();

		void Simulate(float elapsedTime);

		void AddActor(physx::PxActor* actor, entt::entity entity);
		void RemoveActor(physx::PxActor* actor);

		void SetTriggerCallback(TriggerCallback callback);

		physx::PxController* CreateController(float height, float radius, glm::vec3 position);

	private:
		physx::PxScene* m_PxScene;
		physx::PxControllerManager* m_PxControllerManager;
		
		std::map<physx::PxActor*, entt::entity> m_Entities;

		// Callbacks
		TriggerCallback m_TriggerCallback;

		// Inherited via PxSimulationEventCallback
		virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override {}
		virtual void onWake(physx::PxActor** actors, physx::PxU32 count) override {}
		virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) override {}
		virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override {}
		virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
		virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override {}
	};

}