#include "egpch.h"
#include "PhysicsMesh.h"
#include "PhysicsAPI.h"

Engine::Physics::PsMesh::PsMesh(const std::vector<glm::vec3>& positions, std::vector<uint32_t>& indices)
	: m_Positions(positions), m_Indices(indices), m_Vertices(positions.size()), m_Faces(indices.size() / 3)
{
}

physx::PxTriangleMesh* Engine::Physics::PsMesh::GetPxTriangleMesh()
{
	if (!HasPxTriangleMesh())
		InitPxTriangleMesh();
	return m_PxTriangleMesh;
}

void Engine::Physics::PsMesh::InitPxTriangleMesh()
{
	m_PxTriangleMesh = PhysicsAPI::CreateTriangleMesh(this);
}
