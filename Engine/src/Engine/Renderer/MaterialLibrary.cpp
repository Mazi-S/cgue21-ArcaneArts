#include "egpch.h"

#include "MaterialLibrary.h"
#include "ShaderLibrary.h"
#include "TextureLibrary.h"

#include "Engine/Util/Serialization.h"

namespace Engine {

	std::map<std::string, Ref<Material>> MaterialLibrary::s_Materials;
	Ref<Material> MaterialLibrary::s_Default;

	void MaterialLibrary::Init()
	{
		s_Default = CreateRef<Material>(MaterialProperties("DefaultMaterial"));
		Load();
	}

	void MaterialLibrary::Load(const std::string& filepath)
	{
		Deserialize(filepath);
	}

	void MaterialLibrary::Save(const std::string& filepath)
	{
		Serialize(filepath);
	}

	Ref<Material> MaterialLibrary::Create(MaterialProperties& spec)
	{
		Ref<Material> material = CreateRef<Material>(spec);
		s_Materials[material->GetName()] = material;
		return material;
	}

	Ref<Material> MaterialLibrary::Create(const std::string& name)
	{
		return Create(MaterialProperties(name));
	}

	Ref<Material> MaterialLibrary::Get(const std::string& name)
	{
		if (!Contains(name))
			return s_Default;
		return s_Materials[name];
	}

	std::vector<std::string> MaterialLibrary::GetNames()
	{
		std::vector<std::string> names;
		for (auto entry : s_Materials)
			names.push_back(entry.first);
		return names;
	}

	bool MaterialLibrary::Contains(const std::string& name)
	{
		return s_Materials.find(name) != s_Materials.end();
	}

	void MaterialLibrary::Remove(const std::string& name)
	{
		s_Materials.erase(name);
	}

	void MaterialLibrary::Rename(const std::string oldName, const std::string newName)
	{
		ASSERT(!Contains(newName), "Material already exists!");
		s_Materials[newName] = s_Materials[oldName];
		s_Materials[newName]->m_Name = newName;
		Remove(oldName);
	}

	void MaterialLibrary::Add(const Ref<Material>& material)
	{
		s_Materials[material->GetName()] = material;
	}

	// Serialization
	static void SerializeMaterial(YAML::Emitter& out, Ref<Material> material);
	static Ref<MaterialProperties> DeserializeMaterial(const YAML::Node& material);

	void MaterialLibrary::Serialize(const std::string& filepath)
	{
		LOG_INFO("Serialize MaterialLibrary...\n\tfile: {}", filepath);

		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "MaterialLibrary" << YAML::Value << "Untitled";

		// Materials
		out << YAML::Key << "Materials" << YAML::Value << YAML::BeginSeq;
		for (auto& entry : s_Materials)
			SerializeMaterial(out, entry.second);
		out << YAML::EndSeq;

		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void MaterialLibrary::Deserialize(const std::string& filepath)
	{
		LOG_INFO("Deserialize MaterialLibrary...\n\tfile: {}", filepath);

		YAML::Node data = YAML::LoadFile(filepath);

		ASSERT(data["MaterialLibrary"], "No MaterialLibrary node!");

		if (data["Materials"])
			for (auto materialNode : data["Materials"])
			{
				Ref<MaterialProperties> spec = DeserializeMaterial(materialNode);
				Create(*spec);
			}
	}

	static void SerializeMaterial(YAML::Emitter& out, Ref<Material> material)
	{
		out << YAML::BeginMap; // Material
		out << YAML::Key << "Material";
		out << YAML::Value << material->GetName();

		out << YAML::Key << "Shader";
		out << YAML::Value << material->GetShader();

		out << YAML::Key << "Ambient";
		out << YAML::Value << material->GetAmbient();

		out << YAML::Key << "Diffuse";
		out << YAML::Value << material->GetDiffuse();

		out << YAML::Key << "Specular";
		out << YAML::Value << material->GetSpecular();

		out << YAML::Key << "Shininess";
		out << YAML::Value << material->GetShininess();

		// Textures
		if (!material->GetTextures().empty())
		{
			out << YAML::Key << "Textures";
			out << YAML::Value << YAML::BeginMap; 

			for (auto& entry : material->GetTextures())
				out << YAML::Key << entry.first << YAML::Value << entry.second;
			
			out << YAML::EndMap;
		}

		out << YAML::EndMap; // Material
	}

	static Ref<MaterialProperties> DeserializeMaterial(const YAML::Node& material)
	{
		if (!material["Material"] || !material["Shader"] || !material["Ambient"] || !material["Diffuse"] || !material["Specular"] || !material["Shininess"])
			return nullptr;

		std::string name = material["Material"].as<std::string>();
		std::string shader = material["Shader"].as<std::string>();

		glm::vec3 ambient = material["Ambient"].as<glm::vec3>();
		glm::vec3 diffuse = material["Diffuse"].as<glm::vec3>();
		glm::vec3 specular = material["Specular"].as<glm::vec3>();
		float shininess = material["Shininess"].as<float>();

		Ref<MaterialProperties> spec = CreateRef<MaterialProperties>(name, ambient, diffuse, specular, shininess, shader);

		if (material["Textures"])
			for (auto textureNode : material["Textures"])
			{
				uint16_t slot = textureNode.first.as<uint16_t>();
				std::string texture = textureNode.second.as<std::string>();
				spec->Textures[slot] = texture;
			}

		return spec;
	}
}

