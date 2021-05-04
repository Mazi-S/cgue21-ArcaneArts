#pragma once
#include "Engine/Core/Base.h"

#include <glm/glm.hpp>
#include <string>
#include <map>
#include <vector>

#include "Platform/OpenGL/OpenGLShader.h"

namespace Engine {

	class ShaderLibrary
	{
		friend class ShaderPanel;

	public:
		static void Init();

		static void Load(const std::string& filepath = "assets/ShaderLibrary.yaml");
		static void Save(const std::string& filepath = "assets/ShaderLibrary.yaml");

		static Ref<OpenGL::GlShader> Create(const std::string& name, const std::string& filepath);

		static Ref<OpenGL::GlShader> Get(const std::string& name = std::string());
		static std::vector<std::string> GetNames();

		static bool Contains(const std::string& name);

		static void Remove(const std::string& name);
		static void Rename(const std::string oldName, const std::string newName);

	private:
		static void Add(const Ref<OpenGL::GlShader>& shader);

		static void Serialize(const std::string& filepath);
		static void Deserialize(const std::string& filepath);

	private:
		static std::map<std::string, Ref<OpenGL::GlShader>> s_Shaders;
		static Ref<OpenGL::GlShader> s_Default;
	};

}