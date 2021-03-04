#include "egpch.h"
#include "PhysicsScene.h"
#include "Engine/Physics/PhysicsAPI.h"

namespace Engine::Physics {

	PsScene::PsScene()
	{
		m_PxScene = PhysicsAPI::CreateScene();
		m_PxControllerManager = PxCreateControllerManager(*m_PxScene);
		m_PxScene->setSimulationEventCallback(this);
	}

	void PsScene::Simulate(float elapsedTime)
	{
		m_PxScene->simulate(elapsedTime);
		m_PxScene->fetchResults(true);
	}

	void PsScene::AddActor(physx::PxActor* actor, entt::entity entity)
	{
		m_PxScene->addActor(*actor);
		m_Entities[actor] = entity;
	}

	void PsScene::RemoveActor(physx::PxActor* actor)
	{
		m_Entities.erase(actor);
		m_PxScene->removeActor(*actor);
	}

	void PsScene::SetTriggerCallback(TriggerCallback callback)
	{
		m_TriggerCallback = callback;
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

			entt::entity triggerEntity = m_Entities[pairs[i].triggerActor];
			entt::entity otherEntity = m_Entities[pairs[i].otherActor];

			m_TriggerCallback(triggerEntity, otherEntity);
			LOG_INFO("onTrigger");
		}
	}

}

