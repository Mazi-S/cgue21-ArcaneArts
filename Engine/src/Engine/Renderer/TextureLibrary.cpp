#include "egpch.h"
#include "TextureLibrary.h"

#include "Engine/Util/Serialization.h"

namespace Engine {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Texture 2D Library /////////////////////////////////////////////////////////////////////////

	std::unordered_map<std::string, Ref<OpenGL::GlTexture2D>> Texture2DLibrary::s_Textures2D;
	Ref<OpenGL::GlTexture2D> Texture2DLibrary::s_Default;

	void Texture2DLibrary::Init()
	{
		OpenGL::Texture2DSpecification spec;
		spec.Width = 1;
		spec.Height = 1;
		spec.Internalformat = 0x8058;
		spec.Format = 0x1908;
		spec.Type = 0x1401;

		s_Default = CreateRef<OpenGL::GlTexture2D>("DefaultTexture", spec);
		uint32_t textureData = 0xffffffff;
		s_Default->SetData(&textureData, sizeof(uint32_t));

		Load();
	}

	void Texture2DLibrary::Load(const std::string& filepath)
	{
		Deserialize(filepath);
	}

	void Texture2DLibrary::Save(const std::string& filepath)
	{
		Serialize(filepath);
	}

	void Texture2DLibrary::Add(const Ref<OpenGL::GlTexture2D>& texture)
	{
		auto& name = texture->GetName();
		ASSERT(!ContainsTexture2D(name), "Texture already exists!");
		s_Textures2D[name] = texture;
	}

	Ref<OpenGL::GlTexture2D> Texture2DLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto texture = CreateRef<OpenGL::GlTexture2D>(name, filepath);
		Add(texture);
		return texture;
	}

	Ref<OpenGL::GlTexture2D> Texture2DLibrary::Get(const std::string& name)
	{
		if (!ContainsTexture2D(name))
			return s_Default;
		return s_Textures2D[name];
	}

	std::vector<std::string> Texture2DLibrary::GetNames()
	{
		std::vector<std::string> names;
		for (auto entry : s_Textures2D)
			names.push_back(entry.first);
		return names;
	}

	bool Texture2DLibrary::ContainsTexture2D(const std::string& name)
	{
		return s_Textures2D.find(name) != s_Textures2D.end();
	}

	// Serialization
	static void SerializeTexture2D(YAML::Emitter& out, Ref<OpenGL::GlTexture2D> texture);
	static Ref<OpenGL::GlTexture2D> DeserializeTexture2D(const YAML::Node& texture);

	void Texture2DLibrary::Serialize(const std::string& filepath)
	{
		LOG_INFO("Serialize Texture2DLibrary...\n\tfile: {}", filepath);

		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "Texture2DLibrary" << YAML::Value << "Untitled";

		// Textures
		out << YAML::Key << "Textures" << YAML::Value << YAML::BeginSeq;
		for (auto& entry : s_Textures2D)
			SerializeTexture2D(out, entry.second);
		out << YAML::EndSeq;

		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void Texture2DLibrary::Deserialize(const std::string& filepath)
	{
		LOG_INFO("Deserialize Texture2DLibrary...\n\tfile: {}", filepath);

		YAML::Node data = YAML::LoadFile(filepath);

		ASSERT(data["Texture2DLibrary"], "No Texture2DLibrary node!");

		if (data["Textures"])
			for (auto textureNode : data["Textures"])
			{
				Ref<OpenGL::GlTexture2D> texture = DeserializeTexture2D(textureNode);
				Add(texture);
			}
	}

	static void SerializeTexture2D(YAML::Emitter& out, Ref<OpenGL::GlTexture2D> texture)
	{
		if (texture->IsDynamic())
			return;

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

	static Ref<OpenGL::GlTexture2D> DeserializeTexture2D(const YAML::Node& texture)
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

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Cube Texture Library ///////////////////////////////////////////////////////////////////////

	std::unordered_map<std::string, Ref<OpenGL::GlCubeTexture>> CubeTextureLibrary::s_CubeTextures;

	void CubeTextureLibrary::Add(const Ref<OpenGL::GlCubeTexture>& texture)
	{
		auto& name = texture->GetName();
		ASSERT(!ContainsCubeTexture(name), "Texture already exists!");
		s_CubeTextures[name] = texture;
	}
	
	Ref<OpenGL::GlCubeTexture> CubeTextureLibrary::LoadCubeTexture(const std::string& name, std::vector<std::string> paths)
	{
		auto texture = CreateRef<OpenGL::GlCubeTexture>(name, paths);
		Add(texture);
		return texture;
	}
	
	Ref<OpenGL::GlCubeTexture> CubeTextureLibrary::GetCubeTexture(const std::string& name)
	{
		ASSERT(ContainsCubeTexture(name), "Texture not found!");
		return s_CubeTextures[name];
	}
	
	bool CubeTextureLibrary::ContainsCubeTexture(const std::string& name)
	{
		return s_CubeTextures.find(name) != s_CubeTextures.end();
	}
}
