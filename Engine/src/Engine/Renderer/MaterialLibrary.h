#pragma once
#include "Engine/Core/Base.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Engine/Renderer/Material.h"

namespace Engine {

	class MaterialLibrary
	{
		friend class MaterialPanel;

	public:
		static void Load(const std::string& filepath);
		static void Save(const std::string& filepath);

		static Ref<Material> Create(MaterialProperties& properties);

		static Ref<Material> Get(const std::string& name);
		static bool Contains(const std::string& name);

	private:
		static void Add(const Ref<Material>& material);

		static void Serialize(const std::string& filepath);
		static void Deserialize(const std::string& filepath);

	private:
		static std::unordered_map<std::string, Ref<Material>> s_Materials;
	};

}