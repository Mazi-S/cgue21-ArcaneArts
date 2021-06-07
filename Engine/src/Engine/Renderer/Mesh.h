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
		std::vector<uint32_t> tangentIndex;
	};

	class Mesh
	{
		friend class MeshLibrary;
		friend class MeshPanel;

	public:
		Mesh() = default;
		Mesh(const std::string& name, const std::string& path, bool tangentSpace = false);
		
		std::string GetName() { return m_Name; }
		std::string GetPath() { return m_Path; }
		bool TangentSpace() { return m_TangentSpace; }

		Ref<Physics::PsMesh> GetPsMesh() { return m_PxMesh; }
		Ref<OpenGL::GlMesh> GetGlMesh() { return m_GlMesh; }

		operator Ref<OpenGL::GlMesh>() { return m_GlMesh; }
		operator Ref<Physics::PsMesh>() { return m_PxMesh; }

	private:
		OpenGL::GlMesh* CreateGlMesh(bool positions, bool texcoords, bool normals, bool tangentSpace);
		Physics::PsMesh* CreatePsMesh();

		void Reload();

	private:
		std::string m_Name;
		std::string m_Path;
		bool m_TangentSpace;

		std::vector<glm::vec3> m_Positions;
		std::vector<glm::vec3> m_Normals;
		std::vector<glm::vec2> m_TextureCoordinates;

		std::vector<glm::vec3> m_Tangent;
		std::vector<glm::vec3> m_Bitangent;

		std::vector<Submesh> m_Submeshes;

		Ref<OpenGL::GlMesh> m_GlMesh;
		Ref<Physics::PsMesh> m_PxMesh;
	};

}


