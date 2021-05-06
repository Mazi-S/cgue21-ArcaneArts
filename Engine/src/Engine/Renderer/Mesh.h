#pragma once
#include "Engine/Core/Base.h"
#include "Platform/OpenGL/OpenGLMesh.h"
#include "Engine/Physics/PhysicsMesh.h"
#include <glm/glm.hpp>

namespace Engine {

	struct Face;

	struct Submesh
	{
		std::vector<Face> Faces;
	};

	struct Face
	{
		uint16_t vertices;
		std::vector<uint32_t> positionIndex;
		std::vector<uint32_t> normalIndex;
		std::vector<uint32_t> textureCoordinateIndex;
	};

	class Mesh
	{
		friend class MeshLibrary;

	public:
		Mesh() = default;
		Mesh(const std::string& name, const std::string& path, std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& textureCoordinates, std::vector<Submesh>& submeshes);
		
		std::string GetName() { return m_Name; }
		std::string GetPath() { return m_Path; }

		Ref<Physics::PsMesh> GetPsMesh() { return m_PxMesh; }
		Ref<OpenGL::GlMesh> GetGlMesh() { return m_GlMesh; }

		operator Ref<OpenGL::GlMesh>() { return m_GlMesh; }

	private:
		Ref<OpenGL::GlMesh> CreateGlMesh(bool positions, bool texcoords, bool normals, OpenGL::GlVertexBufferLayout layout);
		Ref<Physics::PsMesh> CreatePsMesh();

	private:
		std::string m_Name;
		std::string m_Path;

		const std::vector<glm::vec3> m_Positions;
		const std::vector<glm::vec3> m_Normals;
		const std::vector<glm::vec2> m_TextureCoordinates;
		const std::vector<Submesh> m_Submeshes;

		Ref<OpenGL::GlMesh> m_GlMesh;
		Ref<Physics::PsMesh> m_PxMesh;
	};

}


