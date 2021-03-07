#include "egpch.h"
#include "ShaderLibrary.h"

namespace Engine {

	std::unordered_map<std::string, Ref<OpenGL::GlShader>> ShaderLibrary::s_Shaders;

	void ShaderLibrary::Add(const Ref<OpenGL::GlShader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
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

}


