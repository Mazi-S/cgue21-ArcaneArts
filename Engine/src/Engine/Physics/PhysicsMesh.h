#pragma once

#include <glm/glm.hpp>

namespace Engine::Physics {

	class PxMesh
	{
	public:
		PxMesh(const std::vector<glm::vec3>& positions, std::vector<uint32_t>& indices);

		uint32_t VertexCount() { return m_Vertices; }
		uint32_t FaceCount() { return m_Faces; }

		const std::vector<glm::vec3> Vertices() { return m_Positions; }
		const std::vector<uint32_t> Indices() { return m_Indices; }


	private:
		const uint32_t m_Vertices;
		const uint32_t m_Faces;
		
		const std::vector<glm::vec3> m_Positions;
		const std::vector<uint32_t> m_Indices;
	};
}