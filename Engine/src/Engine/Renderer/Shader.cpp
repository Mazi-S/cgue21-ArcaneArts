#include "egpch.h"
#include "Shader.h"

namespace Engine {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// ShaderLibrary //////////////////////////////////////////////////////////////////////////////

	std::unordered_map<std::string, Ref<Shader>> ShaderLibrary::s_Shaders;

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		ASSERT(!Exists(name), "Shader already exists!");
		s_Shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		ASSERT(Exists(name), "Shader not found!");
		return s_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name)
	{
		return s_Shaders.find(name) != s_Shaders.end();
	}

}


