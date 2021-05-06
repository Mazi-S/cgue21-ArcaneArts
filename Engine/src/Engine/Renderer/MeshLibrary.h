#pragma once
#include "Engine/Core/Base.h"

#include <string>
#include <map>

#include "Mesh.h"
namespace Engine {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// MeshLibrary ////////////////////////////////////////////////////////////////////////////////

	class MeshLibrary
	{
		friend class MeshPanel;

	public:
		static void Init();

		static void Load(const std::string& filepath = "assets/MeshLibrary.yaml");
		static void Save(const std::string& filepath = "assets/MeshLibrary.yaml");

		static Ref<Mesh> Create(const std::string& name, const std::string& filepath);

		static Ref<Mesh> Get(const std::string& name = std::string());
		static std::vector<std::string> GetNames();

		static bool Contains(const std::string& name);

		static void Remove(const std::string& name);
		static void Rename(const std::string oldName, const std::string newName);
	private:
		static void Add(const Ref<Mesh>& mesh);

		static void Serialize(const std::string& filepath);
		static void Deserialize(const std::string& filepath);

	private:

		static std::map<std::string, Ref<Mesh>> s_Meshes;
		static Ref<Mesh> s_Default;
	};

}