#include "egpch.h"
#include "ShaderLibrary.h"

#include <fstream>
#include <yaml-cpp/yaml.h>
#include "Engine/Util/Serialization.h"
#include "Engine/Core/Application.h"

namespace Engine {

	std::unordered_map<std::string, Ref<OpenGL::GlShader>> ShaderLibrary::s_Shaders;

	void ShaderLibrary::Add(const Ref<OpenGL::GlShader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	void ShaderLibrary::Load(const std::string& filepath)
	{
		Deserialize(filepath);
	}

	void ShaderLibrary::Save(const std::string& filepath)
	{
		Serialize(filepath);
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<OpenGL::GlShader>& shader)
	{
		ASSERT(!Exists(name), "Shader already exists!");
		s_Shaders[name] = shader;
	}

	Ref<OpenGL::GlShader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = CreateRef<OpenGL::GlShader>(name, filepath);
		Add(name, shader);
		return shader;
	}

	Ref<OpenGL::GlShader> ShaderLibrary::Get(const std::string& name)
	{
		ASSERT(Exists(name), "Shader not found!");
		return s_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name)
	{
		return s_Shaders.find(name) != s_Shaders.end();
	}

	// Serialization
	static void SerializeShader(YAML::Emitter& out, Ref<OpenGL::GlShader> shader);
	static Ref<OpenGL::GlShader> DeserializeShader(const YAML::Node& shader);

	void ShaderLibrary::Serialize(const std::string& filepath)
	{
		LOG_INFO("Serialize ShaderLibrary...\n\tfile: {}", filepath);

		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "ShaderLibrary" << YAML::Value << "Untitled";

		// Shaders
		out << YAML::Key << "Shaders" << YAML::Value << YAML::BeginSeq;
		for (auto& entry : s_Shaders)
			SerializeShader(out, entry.second);
		out << YAML::EndSeq;

		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void ShaderLibrary::Deserialize(const std::string& filepath)
	{
		LOG_INFO("Deserialize ShaderLibrary...\n\tfile: {}", filepath);

		YAML::Node data = YAML::LoadFile(filepath);

		ASSERT(data["ShaderLibrary"], "No ShaderLibrary node!");

		if (data["Shaders"])
			for (YAML::Node shaderNode : data["Shaders"])
			{
				Ref<OpenGL::GlShader> shader = DeserializeShader(shaderNode);
				Add(shader);
			}
	}

	static void SerializeShader(YAML::Emitter& out, Ref<OpenGL::GlShader> shader)
	{
		out << YAML::BeginMap; // Shader
		out << YAML::Key << "Shader";
		out << YAML::Value << shader->GetName();

		out << YAML::Key << "Path";
		out << YAML::Value << shader->GetPath();

		out << YAML::EndMap; // Shader
	}

	static Ref<OpenGL::GlShader> DeserializeShader(const YAML::Node& shader)
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

}


