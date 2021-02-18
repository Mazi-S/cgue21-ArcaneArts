#include "egpch.h"
#include "Physics.h"
#include "Engine/Util/Math.h"

physx::PxDefaultAllocator s_AllocatorCallback;
physx::PxDefaultErrorCallback s_ErrorCallback;

physx::PxFoundation* s_Foundation;
physx::PxPhysics*  s_PhysicsSDK;
physx::PxCooking* s_Cooking;

physx::PxDefaultCpuDispatcher* s_TaskDispatcher;

namespace Engine {

	void PhysicsAPI::Init()
	{
		s_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, s_AllocatorCallback, s_ErrorCallback);
		ASSERT(s_Foundation, "PxCreateFoundation failed!");

		s_PhysicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *s_Foundation, physx::PxTolerancesScale());
		ASSERT(s_PhysicsSDK, "PxCreatePhysics failed!");

		s_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *s_Foundation, physx::PxCookingParams(s_PhysicsSDK->getTolerancesScale()));
		ASSERT(s_Cooking, "PxCreateCooking failed!");

		s_TaskDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	}

	void PhysicsAPI::Shutdown()
	{
		s_PhysicsSDK->release();
		s_Foundation->release();
	}

	physx::PxRigidDynamic* PhysicsAPI::CreateRegidDynamicSphere(glm::vec3 position, float radius)
	{
		static physx::PxMaterial* material = s_PhysicsSDK->createMaterial(0.5f, 0.5f, 0.6f);
		physx::PxShape* shape = s_PhysicsSDK->createShape(physx::PxSphereGeometry(radius), *material);

		physx::PxRigidDynamic* actor = s_PhysicsSDK->createRigidDynamic(physx::PxTransform({ position.x, position.y, position.z }));
		actor->attachShape(*shape);

		shape->release();

		return actor;
	}

	physx::PxRigidDynamic* PhysicsAPI::CreateRegidDynamic()
	{
		static physx::PxMaterial* material = s_PhysicsSDK->createMaterial(0.5f, 0.5f, 0.6f);
		static physx::PxShape* shape = s_PhysicsSDK->createShape(physx::PxBoxGeometry(1, 1, 1), *material);
		physx::PxTransform transform((rand() % 10) / 10.0f, rand() % 50, -25);
		
		physx::PxRigidDynamic* body = s_PhysicsSDK->createRigidDynamic(transform);
		body->attachShape(*shape);
		//physx::PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
		//s_Scene->addActor(*body);
		//shape->release();


		return body;
	}

	physx::PxController* PhysicsAPI::CreateController(physx::PxControllerManager* manager, glm::vec3 position)
	{
		static physx::PxMaterial* material = s_PhysicsSDK->createMaterial(0.5f, 0.5f, 0.6f);
		physx::PxCapsuleControllerDesc desc;

		{
			desc.position = physx::PxExtendedVec3(position.x, position.y, position.z);
			desc.radius = 0.20f;
			desc.height = 1.6f;
			desc.contactOffset = 0.05f;
			desc.stepOffset = 0.01f;
			desc.slopeLimit = cosf(glm::radians(25.0f));
			desc.upDirection = physx::PxVec3(0, 1, 0);
			desc.material = material;
		}

		physx::PxController* c = manager->createController(desc);
		return c;
	}

	physx::PxRigidStatic* PhysicsAPI::CreateRigidStatic(Ref<Mesh> mesh, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	{
		static physx::PxMaterial* material = s_PhysicsSDK->createMaterial(0.5f, 0.5f, 0.6f);

		Ref<Physics::PxMesh> pxMesh = mesh->GetPxMesh();
		const std::vector<uint32_t> indices = pxMesh->Indices();
		const std::vector<glm::vec3> vertices = pxMesh->Vertices();

		physx::PxTriangleMeshDesc meshDesc;
		meshDesc.points.count = pxMesh->VertexCount();
		meshDesc.points.stride = sizeof(physx::PxVec3);
		meshDesc.points.data = vertices.data();

		meshDesc.triangles.count = pxMesh->FaceCount();
		meshDesc.triangles.stride = 3 * sizeof(uint32_t);
		meshDesc.triangles.data = indices.data();

		ASSERT(s_Cooking->validateTriangleMesh(meshDesc), "Invalid triangle mesh!");

		physx::PxTriangleMesh* triMesh = s_Cooking->createTriangleMesh(meshDesc, s_PhysicsSDK->getPhysicsInsertionCallback());

		physx::PxTriangleMeshGeometry geometry = physx::PxTriangleMeshGeometry(triMesh);
		geometry.scale = physx::PxVec3({ scale.x, scale.y, scale.z });
		physx::PxShape* shape = s_PhysicsSDK->createShape(geometry, *material);

		physx::PxTransform transform({ position.x, position.y, position.z }, Util::Math::ToQuaternion(rotation));
		physx::PxRigidStatic* body = s_PhysicsSDK->createRigidStatic(transform);
		body->attachShape(*shape);
		shape->release();
		return body;
	}

	physx::PxScene* PhysicsAPI::CreateScene()
	{
		physx::PxSceneDesc sceneDescription(s_PhysicsSDK->getTolerancesScale());
		sceneDescription.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
		sceneDescription.cpuDispatcher = s_TaskDispatcher;
		sceneDescription.filterShader = physx::PxDefaultSimulationFilterShader;
		physx::PxScene* scene = s_PhysicsSDK->createScene(sceneDescription);

		// todo: just for testing
			physx::PxMaterial* material = s_PhysicsSDK->createMaterial(0.5f, 0.5f, 0.6f);
			physx::PxRigidStatic* groundPlane = physx::PxCreatePlane(*s_PhysicsSDK, physx::PxPlane(0, 1, 0, 0), *material);
			scene->addActor(*groundPlane);

		return scene;
	}

}