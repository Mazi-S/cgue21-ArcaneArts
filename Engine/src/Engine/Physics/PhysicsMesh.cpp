#include "egpch.h"
#include "PhysicsMesh.h"

Engine::Physics::PxMesh::PxMesh(const std::vector<glm::vec3>& positions, std::vector<uint32_t>& indices)
	: m_Positions(positions), m_Indices(indices), m_Vertices(positions.size()), m_Faces(indices.size() / 3)
{
}
