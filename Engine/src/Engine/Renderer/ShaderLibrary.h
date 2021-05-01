#pragma once
#include "Engine/Core/Base.h"

#include <glm/glm.hpp>
#include <unordered_map>
#include "Platform/OpenGL/OpenGLShader.h"

namespace Engine {

	class ShaderLibrary
	{
		friend class ShaderPanel;

	public:
		static void Load(const std::string& filepath);
		static void Save(const std::string& filepath);

		static void Add(const std::string& name, const Ref<OpenGL::GlShader>& shader);
		static void Add(const Ref<OpenGL::GlShader>& shader);
		static Ref<OpenGL::GlShader> Load(const std::string& name, const std::string& filepath);

		static Ref<OpenGL::GlShader> Get(const std::string& name);
		static bool Exists(const std::string& name);

	private:
		static void Serialize(const std::string& filepath);
		static void Deserialize(const std::string& filepath);

	private:
		static std::unordered_map<std::string, Ref<OpenGL::GlShader>> s_Shaders;
	};

}