#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/VertexArray.h"
#include "Platform/OpenGL/OpenGLMesh.h"
#include "Engine/Physics/PhysicsMesh.h"
#include "glm/glm.hpp"

namespace Engine {

	class Submesh;

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Mesh ///////////////////////////////////////////////////////////////////////////////////////

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
	public:
		Mesh() = default;
		Mesh(const std::string& name, std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& textureCoordinates, std::vector<Submesh>& submeshes);

		
		std::string GetName() { return m_Name; }

		Ref<Physics::PxMesh> GetPxMesh() { return m_PxMesh; }
		Ref<OpenGL::GlMesh> GetGlMesh() { return m_GlMesh; }

		operator Ref<OpenGL::GlMesh>() { return m_GlMesh; }

	private:
		Ref<OpenGL::GlMesh> CreateGlMesh(bool positions, bool texcoords, bool normals, VertexBufferLayout layout);
		Ref<Physics::PxMesh> CreatePxMesh();

	private:
		const std::string m_Name;
		const std::vector<glm::vec3> m_Positions;
		const std::vector<glm::vec3> m_Normals;
		const std::vector<glm::vec2> m_TextureCoordinates;
		const std::vector<Submesh> m_Submeshes;

		Ref<OpenGL::GlMesh> m_GlMesh;
		Ref<Physics::PxMesh> m_PxMesh;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// MeshLibrary ////////////////////////////////////////////////////////////////////////////////

	class MeshLibrary
	{
	public:
		static void Add(const Ref<Mesh>& mesh);
		static Ref<Mesh> Load(const std::string& name, const std::string& filepath);

		static Ref<Mesh> Get(const std::string& name);
		static bool Exists(const std::string& name);

	private:
		static std::unordered_map<std::string, Ref<Mesh>> s_Meshes;
	};
}


