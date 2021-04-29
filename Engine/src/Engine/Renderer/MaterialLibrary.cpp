#include "egpch.h"
#include "Material.h"

#include "Engine/Core/Application.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace YAML {

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}

namespace Engine {

	std::unordered_map<std::string, Ref<Material>> MaterialLibrary::s_Materials;
	std::unordered_map<std::string, Ref<OpenGL::GlShader>> MaterialLibrary::s_Shaders;
	std::unordered_map<std::string, Ref<OpenGL::GlTexture2D>> MaterialLibrary::s_Textures;

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
		auto shader = s_Shaders[spec.Shader];

		Ref<Material> material;
		if (spec.Textures[0] != "")
			material = CreateRef<TextureMaterial>(spec, s_Textures[spec.Textures[0]], shader);
		else
			material = CreateRef<Material>(spec, shader);

		s_Materials[material->GetName()] = material;
		return material;
	}

	bool Engine::MaterialLibrary::AddShader(const Ref<OpenGL::GlShader>& shader)
	{
		if (s_Shaders[shader->GetName()] != nullptr)
			return false;

		s_Shaders[shader->GetName()] = shader;
		return true;
	}

	bool Engine::MaterialLibrary::AddTexture(const Ref<OpenGL::GlTexture2D>& colorTex)
	{
		if (s_Textures[colorTex->GetName()] != nullptr)
			return false;

		s_Textures[colorTex->GetName()] = colorTex;
		return true;
	}

	Ref<Material> MaterialLibrary::Get(const std::string& name)
	{
		ASSERT(Contains(name), "Material not found!");
		return s_Materials[name];
	}

	bool MaterialLibrary::Contains(const std::string& name)
	{
		return s_Materials.find(name) != s_Materials.end();
	}

	void MaterialLibrary::Add(const Ref<Material>& material)
	{
		s_Materials[material->GetName()] = material;
	}

	// Serialization
	void SerializeMaterial(YAML::Emitter& out, Ref<Material> material);
	void SerializeShader(YAML::Emitter& out, Ref<OpenGL::GlShader> shader);
	void SerializeTexture(YAML::Emitter& out, Ref<OpenGL::GlTexture2D> texture);

	Ref<MaterialProperties> DeserializeMaterial(const YAML::Node& material);
	Ref<OpenGL::GlShader> DeserializeShader(const YAML::Node& shader);
	Ref<OpenGL::GlTexture2D> DeserializeTexture(const YAML::Node& texture);


	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}


	void MaterialLibrary::Serialize(const std::string& filepath)
	{
		LOG_INFO("Serialize MaterialLibrary file:{} ...", filepath);

		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "MaterialLibrary" << YAML::Value << "Untitled";

		// Materials
		out << YAML::Key << "Materials" << YAML::Value << YAML::BeginSeq;
		for (auto& entry : s_Materials)
			SerializeMaterial(out, entry.second);
		out << YAML::EndSeq;

		// Shaders
		out << YAML::Key << "Shaders" << YAML::Value << YAML::BeginSeq;
		for (auto& entry : s_Shaders)
			SerializeShader(out, entry.second);
		out << YAML::EndSeq;

		// Textures
		out << YAML::Key << "Textures" << YAML::Value << YAML::BeginSeq;
		for (auto& entry : s_Textures)
			SerializeTexture(out, entry.second);
		out << YAML::EndSeq;

		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void MaterialLibrary::Deserialize(const std::string& filepath)
	{
		LOG_INFO("Deserialize MaterialLibrary file:{} ...", filepath);

		YAML::Node data = YAML::LoadFile(filepath);

		ASSERT(data["MaterialLibrary"], "No MaterialLibrary node!");

		if (data["Shaders"])
			for (YAML::Node shaderNode : data["Shaders"])
			{
				Ref<OpenGL::GlShader> shader = DeserializeShader(shaderNode);
				AddShader(shader);
			}

		if (data["Textures"])
			for (auto textureNode : data["Textures"])
			{
				Ref<OpenGL::GlTexture2D> texture = DeserializeTexture(textureNode);
				AddTexture(texture);
			}

		if (data["Materials"])
			for (auto materialNode : data["Materials"])
			{
				Ref<MaterialProperties> spec = DeserializeMaterial(materialNode);
				Create(*spec);
			}
	}

	void SerializeMaterial(YAML::Emitter& out, Ref<Material> material)
	{
		out << YAML::BeginMap; // Material
		out << YAML::Key << "Material";
		out << YAML::Value << material->GetName();

		out << YAML::Key << "Shader";
		out << YAML::Value << material->GetShader()->GetName();

		out << YAML::Key << "Ambient";
		glm::vec3 ambient = material->GetAmbient();
		out << YAML::Value << ambient;

		out << YAML::Key << "Diffuse";
		glm::vec3 diffuse = material->GetAmbient();
		out << YAML::Value << diffuse;

		out << YAML::Key << "Specular";
		glm::vec3 specular = material->GetAmbient();
		out << YAML::Value << specular;

		out << YAML::Key << "Shininess";
		out << YAML::Value << material->GetShininess();

		// Textures
		if (!material->GetTextures().empty())
		{
			out << YAML::Key << "Textures";
			out << YAML::Value << YAML::BeginMap; 

			for (auto& entry : material->GetTextures())
				out << YAML::Key << entry.first << YAML::Value << entry.second->GetName();
			
			out << YAML::EndMap;
		}

		out << YAML::EndMap; // Material
	}

	void Engine::SerializeShader(YAML::Emitter& out, Ref<OpenGL::GlShader> shader)
	{
		out << YAML::BeginMap; // Shader
		out << YAML::Key << "Shader";
		out << YAML::Value << shader->GetName();

		out << YAML::Key << "Path";
		out << YAML::Value << shader->GetPath();

		out << YAML::EndMap; // Shader
	}

	void Engine::SerializeTexture(YAML::Emitter& out, Ref<OpenGL::GlTexture2D> texture)
	{
		out << YAML::BeginMap; // Texture
		out << YAML::Key << "Texture";
		out << YAML::Value << texture->GetName();

		out << YAML::Key << "Path";
		out << YAML::Value << texture->GetPath();

		out << YAML::Key << "Specification";
		auto& spec = texture->GetSpecification();
		out << YAML::Value << YAML::BeginMap; // Specification

			out << YAML::Key << "Width" << YAML::Value << spec.Width;
			out << YAML::Key << "Height" << YAML::Value << spec.Height;
			out << YAML::Key << "Internalformat" << YAML::Value << spec.Internalformat;
			out << YAML::Key << "Format" << YAML::Value << spec.Format;
			out << YAML::Key << "Type" << YAML::Value << spec.Type;
			out << YAML::Key << "Wrap_S" << YAML::Value << spec.Wrap_S;
			out << YAML::Key << "Wrap_T" << YAML::Value << spec.Wrap_T;
			out << YAML::Key << "Border" << YAML::Value << spec.Border;
			out << YAML::Key << "Min_Filter" << YAML::Value << spec.Min_Filter;
			out << YAML::Key << "Mag_Filter" << YAML::Value << spec.Mag_Filter;
			out << YAML::Key << "ComparisonMode" << YAML::Value << spec.ComparisonMode;
			out << YAML::Key << "Mipmaps" << YAML::Value << spec.Mipmaps;

		out << YAML::EndMap; // Specification

		out << YAML::EndMap; // Texture
	}

	Ref<MaterialProperties> DeserializeMaterial(const YAML::Node& material)
	{
		if (!material["Material"] || !material["Shader"] || !material["Ambient"] || !material["Diffuse"] || !material["Specular"] || !material["Shininess"])
			return nullptr;

		std::string name = material["Material"].as<std::string>();
		std::string shader = material["Shader"].as<std::string>();

		glm::vec3 ambient = material["Ambient"].as<glm::vec3>();
		glm::vec3 diffuse = material["Diffuse"].as<glm::vec3>();
		glm::vec3 specular = material["Specular"].as<glm::vec3>();
		float shininess = material["Shininess"].as<float>();

		Ref<MaterialProperties> spec = CreateRef<MaterialProperties>(name, ambient, diffuse, specular, shininess);
		spec->Shader = shader;

		if (material["Textures"])
			for (auto textureNode : material["Textures"])
			{
				uint16_t slot = textureNode.first.as<uint16_t>();
				std::string texture = textureNode.second.as<std::string>();
				spec->Textures[slot] = texture;
			}

		return spec;
	}

	Ref<OpenGL::GlShader> DeserializeShader(const YAML::Node& shader)
	{
		if (!shader["Shader"] || !shader["Path"])
			return nullptr;

		std::string name = shader["Shader"].as<std::string>();
		std::string path = shader["Path"].as<std::string>();

		auto glShader = CreateRef<OpenGL::GlShader>(name, path);
		glShader->Bind();
		glShader->SetFloat("u_Brightness", Engine::Application::Get().Brightness());
		return glShader;
	}

	Ref<OpenGL::GlTexture2D> DeserializeTexture(const YAML::Node& texture)
	{
		if (!texture["Texture"] || !texture["Path"] || !texture["Specification"])
			return nullptr;

		std::string name = texture["Texture"].as<std::string>();
		std::string path = texture["Path"].as<std::string>();

		YAML::Node specNode = texture["Specification"];
		OpenGL::Texture2DSpecification spec;
		
		if (specNode["Width"])
			spec.Width = specNode["Width"].as<uint32_t>();
		if (specNode["Height"])
			spec.Height = specNode["Height"].as<uint32_t>();
		if (specNode["Internalformat"])
			spec.Internalformat = specNode["Internalformat"].as<uint32_t>();
		if (specNode["Format"])
			spec.Format = specNode["Format"].as<uint32_t>();
		if (specNode["Type"])
			spec.Type = specNode["Type"].as<uint32_t>();
		if (specNode["Wrap_S"])
			spec.Wrap_S = specNode["Wrap_S"].as<uint32_t>();
		if (specNode["Wrap_T"])
			spec.Wrap_T = specNode["Wrap_T"].as<uint32_t>();
		if (specNode["Border"])
			spec.Border = specNode["Border"].as<glm::vec4>();
		if (specNode["Min_Filter"])
			spec.Min_Filter = specNode["Min_Filter"].as<uint32_t>();
		if (specNode["Mag_Filter"])
			spec.Mag_Filter = specNode["Mag_Filter"].as<uint32_t>();
		if (specNode["ComparisonMode"])
			spec.ComparisonMode = specNode["ComparisonMode"].as<uint32_t>();
		if (specNode["Mipmaps"])
			spec.Mipmaps = specNode["Mipmaps"].as<bool>();

		return CreateRef<OpenGL::GlTexture2D>(name, spec, path);
	}

}

