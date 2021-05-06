#include "egpch.h"
#include "MeshLibrary.h"

#include <fstream>
#include <yaml-cpp/yaml.h>
#include "Engine/Util/Serialization.h"

namespace Engine {

	std::map<std::string, Ref<Mesh>> MeshLibrary::s_Meshes;
	Ref<Mesh> MeshLibrary::s_Default;

	void MeshLibrary::Init()
	{
		s_Default = CreateRef<Mesh>("DefaultCube", "assets/objects/cube.obj");
		Load();
	}

	void MeshLibrary::Load(const std::string& filepath)
	{
		Deserialize(filepath);
	}

	void MeshLibrary::Save(const std::string& filepath)
	{
		Serialize(filepath);
	}

	Ref<Mesh> MeshLibrary::Create(const std::string& name, const std::string& filepath)
	{
		Ref<Mesh> mesh = CreateRef<Mesh>(name, filepath);
		Add(mesh);
		return mesh;
	}

	Ref<Mesh> MeshLibrary::Get(const std::string& name)
	{
		if (!Contains(name))
			return s_Default;
		return s_Meshes[name];
	}

	std::vector<std::string> MeshLibrary::GetNames()
	{
		std::vector<std::string> names;
		for (auto entry : s_Meshes)
			names.push_back(entry.first);
		return names;
	}

	bool MeshLibrary::Contains(const std::string& name)
	{
		return s_Meshes.find(name) != s_Meshes.end();
	}

	void MeshLibrary::Add(const Ref<Mesh>& mesh)
	{
		auto& name = mesh->GetName();
		ASSERT(!Contains(name), "Mesh already exists!");
		s_Meshes[name] = mesh;
	}

	void MeshLibrary::Rename(const std::string oldName, const std::string newName)
	{
		ASSERT(!Contains(newName), "Mesh already exists!");
		s_Meshes[newName] = s_Meshes[oldName];
		s_Meshes[newName]->m_Name = newName;
		Remove(oldName);
	}

	void MeshLibrary::Remove(const std::string& name)
	{
		s_Meshes.erase(name);
	}

	// Serialization
	static void SerializeMesh(YAML::Emitter& out, Ref<Mesh> mesh);
	static Ref<Mesh> DeserializeMesh(const YAML::Node& mesh);

	void MeshLibrary::Serialize(const std::string& filepath)
	{
		LOG_INFO("Serialize MeshLibrary...\n\tfile: {}", filepath);

		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "MeshLibrary" << YAML::Value << "Untitled";

		// Shaders
		out << YAML::Key << "Meshes" << YAML::Value << YAML::BeginSeq;
		for (auto& entry : s_Meshes)
			SerializeMesh(out, entry.second);
		out << YAML::EndSeq;

		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void MeshLibrary::Deserialize(const std::string& filepath)
	{
		LOG_INFO("Deserialize MeshLibrary...\n\tfile: {}", filepath);

		YAML::Node data = YAML::LoadFile(filepath);

		ASSERT(data["MeshLibrary"], "No MeshLibrary node!");

		if (data["Meshes"])
			for (YAML::Node meshNode : data["Meshes"])
			{
				Ref<Mesh> mesh = DeserializeMesh(meshNode);
				Add(mesh);
			}
	}

	void SerializeMesh(YAML::Emitter& out, Ref<Mesh> mesh)
	{
		out << YAML::BeginMap; // Mesh
		out << YAML::Key << "Mesh";
		out << YAML::Value << mesh->GetName();

		out << YAML::Key << "Path";
		out << YAML::Value << mesh->GetPath();

		out << YAML::EndMap; // Mesh
	}

	Ref<Mesh> DeserializeMesh(const YAML::Node& mesh)
	{
		if (!mesh["Mesh"] || !mesh["Path"])
			return nullptr;

		std::string name = mesh["Mesh"].as<std::string>();
		std::string path = mesh["Path"].as<std::string>();

		return CreateRef<Mesh>(name, path);
	}

}