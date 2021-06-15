#include "ActorFactory.h"

namespace ActorFactory {

	physx::PxRigidDynamic* Monster(std::string meshName, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	{
		static auto* material = Engine::PhysicsAPI::CreateMaterial(.5,.5,.5);

		Engine::Ref<Engine::Physics::PsMesh> mesh = Engine::MeshLibrary::Get(meshName)->GetPsMesh();

		physx::PxRigidDynamic* actor = Engine::PhysicsAPI::CreateRigidDynamic(position, rotation);
		physx::PxShape* shape = Engine::PhysicsAPI::CreateShape(mesh->GetPxTriangleMesh(), scale, material);

		Engine::PhysicsAPI::SetKinematic(actor, true);
		actor->attachShape(*shape);
		shape->release();
		
		return actor;
	}

	physx::PxRigidDynamic* MagicBall(glm::vec3 position, glm::vec3 rotation, float radius)
	{
		static auto* material = Engine::PhysicsAPI::CreateMaterial(.5, .5, .5);

		physx::PxRigidDynamic* actor = Engine::PhysicsAPI::CreateRigidDynamic(position, rotation);

		physx::PxShape* shape = Engine::PhysicsAPI::CreateSphereShape(radius, material);

		shape->setContactOffset(0.1f);

		Engine::PhysicsAPI::SetSimulation(shape, false);
		Engine::PhysicsAPI::SetTrigger(shape, true);
		Engine::PhysicsAPI::SetKinematic(actor, true);

		actor->attachShape(*shape);
		shape->release();

		return actor;
	}

}
