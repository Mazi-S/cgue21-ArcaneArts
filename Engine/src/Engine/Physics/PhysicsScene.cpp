#include "egpch.h"
#include "PhysicsScene.h"
#include "Engine/Physics/PhysicsAPI.h"
#include "Engine/Events/PhysicsEvent.h"

namespace Engine::Physics {

	PsScene::PsScene()
	{
		m_PxScene = PhysicsAPI::CreateScene();
		m_PxControllerManager = PxCreateControllerManager(*m_PxScene);
		m_PxScene->setSimulationEventCallback(this);
	}

	PsScene::~PsScene()
	{
		m_PxControllerManager->release();
	}

	void PsScene::Simulate(float elapsedTime)
	{
		m_PxScene->simulate(elapsedTime);
		m_PxScene->fetchResults(true);
	}

	void PsScene::AddActor(physx::PxActor* actor, Entity entity)
	{
		m_PxScene->addActor(*actor);
		m_Entities[actor] = entity;
	}

	void PsScene::RemoveActor(physx::PxActor* actor)
	{
		m_Entities.erase(actor);
		m_PxScene->removeActor(*actor);
	}

	void PsScene::SetEventCallback(EventCallbackFn callback)
	{
		m_EventCallback = callback;
	}

	physx::PxController* PsScene::CreateController(float h, float r, glm::vec3 pos)
	{
		return PhysicsAPI::CreateController(m_PxControllerManager, h, r, pos);
	}

	void PsScene::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
	{
		for (uint32_t i = 0; i < count; i++)
		{
			// ignore pairs when shapes have been deleted
			if (pairs[i].flags & (physx::PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | physx::PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
				continue;

			Entity triggerEntity = m_Entities[pairs[i].triggerActor];
			Entity otherEntity = m_Entities[pairs[i].otherActor];

			CollisionEvent event(triggerEntity, otherEntity);
			m_EventCallback(event);
		}
	}

}

