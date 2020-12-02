#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/VertexArray.h"

namespace Engine {

	class Submesh;

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Mesh ///////////////////////////////////////////////////////////////////////////////////////

	class Mesh
	{
	public:
		Mesh() = default;
		Mesh(const std::string& name, Ref<VertexBuffer> vertexBuffer);

		void AddSubmesh(std::vector<uint32_t>& indices);
		void AddSubmesh(const Ref<IndexBuffer>& indexBuffer);

		std::string GetName() { return m_Name; }

		const std::vector<Ref<Submesh>>& GetSubmeshes() const { return m_Submeshes; }

	private:
		const std::string m_Name;
		const Ref<VertexBuffer> m_VertexBuffer;
		std::vector<Ref<Submesh>> m_Submeshes;
	};


	///////////////////////////////////////////////////////////////////////////////////////////////
	// Submesh ////////////////////////////////////////////////////////////////////////////////////

	class Submesh
	{
		friend class Mesh;

	public:
		Submesh() = default;
		Submesh(const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer);

		Ref<VertexArray> GetVertexArray() { return m_VertexArray; }

	private:
		Ref<VertexArray> m_VertexArray;
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


