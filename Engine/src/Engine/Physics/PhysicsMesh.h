#pragma once

#include <glm/glm.hpp>
#include <PxPhysicsAPI.h>

namespace Engine::Physics {

	class PsMesh
	{
	public:
		PsMesh(const std::vector<glm::vec3>& positions, std::vector<uint32_t>& indices);

		uint32_t VertexCount() { return m_Vertices; }
		uint32_t FaceCount() { return m_Faces; }

		const std::vector<glm::vec3> Vertices() { return m_Positions; }
		const std::vector<uint32_t> Indices() { return m_Indices; }

		physx::PxTriangleMesh* GetPxTriangleMesh();
	private:
		bool HasPxTriangleMesh() { return m_PxTriangleMesh != nullptr; }
		void InitPxTriangleMesh();

		const uint32_t m_Vertices;
		const uint32_t m_Faces;
		
		const std::vector<glm::vec3> m_Positions;
		const std::vector<uint32_t> m_Indices;

		physx::PxTriangleMesh* m_PxTriangleMesh = nullptr;
	};
}