#pragma once
#include "Engine/Core/Base.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Engine/Renderer/Material.h"

namespace Engine {

	class MaterialLibrary
	{
		friend class MaterialPanel;

	public:
		static void Init();

		static void Load(const std::string& filepath = "assets/MaterialLibrary.yaml");
		static void Save(const std::string& filepath = "assets/MaterialLibrary.yaml");

		static Ref<Material> Create(MaterialProperties& properties);
		static Ref<Material> Create(const std::string& name);

		static Ref<Material> Get(const std::string& name = std::string());
		static std::vector<std::string> GetNames();

		static bool Contains(const std::string& name);

		static void Remove(const std::string& name);
		static void Rename(const std::string oldName, const std::string newName);

	private:
		static void Add(const Ref<Material>& material);

		static void Serialize(const std::string& filepath);
		static void Deserialize(const std::string& filepath);

	private:
		static std::map<std::string, Ref<Material>> s_Materials;
		static Ref<Material> s_Default;
	};

}