#include "egpch.h"
#include "Physics.h"

physx::PxDefaultAllocator s_AllocatorCallback;
physx::PxDefaultErrorCallback s_ErrorCallback;

physx::PxFoundation* s_Foundation;
physx::PxPhysics*  s_PhysicsSDK;
physx::PxCooking* s_Cooking;

physx::PxDefaultCpuDispatcher* s_TaskDispatcher;

namespace Engine {

	void Physics::Init()
	{
		s_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, s_AllocatorCallback, s_ErrorCallback);
		ASSERT(s_Foundation, "PxCreateFoundation failed!");

		s_PhysicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *s_Foundation, physx::PxTolerancesScale());
		ASSERT(s_PhysicsSDK, "PxCreatePhysics failed!");

		s_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *s_Foundation, physx::PxCookingParams(s_PhysicsSDK->getTolerancesScale()));
		ASSERT(s_Cooking, "PxCreateCooking failed!");

		s_TaskDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	}

	void Physics::Shutdown()
	{
		s_PhysicsSDK->release();
		s_Foundation->release();
	}

	physx::PxRigidDynamic* Physics::CreateRegidDynamicSphere(glm::vec3 position, float radius)
	{
		static physx::PxMaterial* material = s_PhysicsSDK->createMaterial(0.5f, 0.5f, 0.6f);
		physx::PxShape* shape = s_PhysicsSDK->createShape(physx::PxSphereGeometry(radius), *material);

		physx::PxRigidDynamic* actor = s_PhysicsSDK->createRigidDynamic(physx::PxTransform({ position.x, position.y, position.z }));
		actor->attachShape(*shape);

		return actor;
	}

	physx::PxRigidDynamic* Physics::CreateRegidDynamic()
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

	physx::PxRigidStatic* Physics::CreateRigidStatic(Ref<Mesh> mesh, glm::vec3 position)
	{
		static physx::PxMaterial* material = s_PhysicsSDK->createMaterial(0.5f, 0.5f, 0.6f);

		static const physx::PxVec3 convexVerts[] = { physx::PxVec3(0,1,0), physx::PxVec3(1,0,0), physx::PxVec3(-1,0,0), physx::PxVec3(0,0,1),  physx::PxVec3(0,0,-1) };
		static const std::vector<uint32_t> convexIndices = { 0,3,1, 0,2,3, 0,4,2, 0,1,4, 1,3,4, 3,2,4 };

		physx::PxTriangleMeshDesc meshDesc;
		meshDesc.points.count = 5;
		meshDesc.points.stride = sizeof(physx::PxVec3);
		meshDesc.points.data = convexVerts;

		meshDesc.triangles.count = 6;
		meshDesc.triangles.stride = 3 * sizeof(uint32_t);
		meshDesc.triangles.data = convexIndices.data();

#ifdef false
		physx::PxTriangleMeshDesc meshDesc;
		meshDesc.points.count = mesh->GetVertices().size() / (3 + 2 + 3);
		meshDesc.points.stride = mesh->GetLayout().GetStride();
		meshDesc.points.data = mesh->GetVertices().data();

		meshDesc.triangles.count = mesh->GetIndices().size() / 3;
		meshDesc.triangles.stride = 3 * sizeof(uint32_t);
		meshDesc.triangles.data = mesh->GetIndices().data();
#endif


		ASSERT(s_Cooking->validateTriangleMesh(meshDesc), "Invalid triangle mesh!");

		physx::PxTriangleMesh* triMesh = s_Cooking->createTriangleMesh(meshDesc, s_PhysicsSDK->getPhysicsInsertionCallback());

		physx::PxTriangleMeshGeometry geometry = physx::PxTriangleMeshGeometry(triMesh);
		physx::PxShape* shape = s_PhysicsSDK->createShape(geometry, *material);

		physx::PxTransform transform({ position.x, position.y, position.z });
		physx::PxRigidStatic* body = s_PhysicsSDK->createRigidStatic(transform);
		body->attachShape(*shape);
		return body;
	}

	physx::PxRigidStatic* CreateRigidStatic(Ref<Mesh> mesh)
	{
		static physx::PxMaterial* material = s_PhysicsSDK->createMaterial(0.5f, 0.5f, 0.6f);

		static const physx::PxVec3 convexVerts[] = { physx::PxVec3(0,1,0), physx::PxVec3(1,0,0), physx::PxVec3(-1,0,0), physx::PxVec3(0,0,1),  physx::PxVec3(0,0,-1) };
		static const std::vector<uint32_t> convexIndices = { 0,3,1, 0,2,3, 0,4,2, 0,1,4, 1,3,4, 3,2,4 };

		physx::PxTriangleMeshDesc meshDesc;
		meshDesc.points.count = 5;
		meshDesc.points.stride = sizeof(physx::PxVec3);
		meshDesc.points.data = convexVerts;

		meshDesc.triangles.count = 6;
		meshDesc.triangles.stride = 3 * sizeof(uint32_t);
		meshDesc.triangles.data = convexIndices.data();

		ASSERT(s_Cooking->validateTriangleMesh(meshDesc), "Invalid triangle mesh!");

		physx::PxTriangleMesh* triMesh = s_Cooking->createTriangleMesh(meshDesc, s_PhysicsSDK->getPhysicsInsertionCallback());

		physx::PxTriangleMeshGeometry geometry = physx::PxTriangleMeshGeometry(triMesh);
		physx::PxShape* shape = s_PhysicsSDK->createShape(geometry, *material);

		physx::PxTransform transform((rand() % 10) / 10.0f, 0, -25);
		physx::PxRigidStatic* body = s_PhysicsSDK->createRigidStatic(transform);
		body->attachShape(*shape);
		return body;
	}

	physx::PxScene* Physics::CreateScene()
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