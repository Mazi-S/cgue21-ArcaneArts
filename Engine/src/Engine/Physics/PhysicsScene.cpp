#include "egpch.h"
#include "PhysicsScene.h"
#include "Engine/Physics/Physics.h"

namespace Engine::Physics {

	PsScene::PsScene()
	{
		m_PxScene = PhysicsAPI::CreateScene();
		m_PxControllerManager = PxCreateControllerManager(*m_PxScene);
	}

	void PsScene::Simulate(float elapsedTime)
	{
		m_PxScene->simulate(elapsedTime);
		m_PxScene->fetchResults(true);
	}

	void PsScene::AddActor(physx::PxActor* actor)
	{
		m_PxScene->addActor(*actor);
	}

	physx::PxController* PsScene::CreateController(float h, float r, glm::vec3 pos)
	{
		return PhysicsAPI::CreateController(m_PxControllerManager, h, r, pos);
	}


}

