#include "egpch.h"
#include "PhysicsMesh.h"
#include "PhysicsAPI.h"

namespace Engine::Physics {

	PsMesh::PsMesh(const std::vector<glm::vec3>& positions, std::vector<uint32_t>& indices)
		: m_Positions(positions), m_Indices(indices), m_Vertices(positions.size()), m_Faces(indices.size() / 3)
	{
	}

	physx::PxTriangleMesh* PsMesh::GetPxTriangleMesh()
	{
		if (!HasPxTriangleMesh())
			InitPxTriangleMesh();
		return m_PxTriangleMesh;
	}

	physx::PxConvexMesh* PsMesh::GetPxConvexMesh()
	{
		if (!HasPxConvexMesh())
			InitPxConvexMesh();
		return m_PxConvexMesh;
	}

	void PsMesh::InitPxTriangleMesh()
	{
		m_PxTriangleMesh = PhysicsAPI::CreateTriangleMesh(this);
	}
	
	void PsMesh::InitPxConvexMesh()
	{
		m_PxConvexMesh = PhysicsAPI::CreateConvexMesh(this);
	}

}
