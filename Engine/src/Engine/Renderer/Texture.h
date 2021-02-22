#pragma once

#include <string>
#include <glm/glm.hpp>
#include "Engine/Core/Base.h"

namespace Engine {


	///////////////////////////////////////////////////////////////////////////////////////////////
	// Texture ////////////////////////////////////////////////////////////////////////////////////

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual const std::string& GetName() const = 0;
		virtual const std::string& GetPath() const = 0;

		static Ref<Texture> Create(const std::string& filepath);
		static Ref<Texture> CreateCube(const std::vector<std::string> faces);
	};


	///////////////////////////////////////////////////////////////////////////////////////////////
	// TextureLibrary /////////////////////////////////////////////////////////////////////////////

	class TextureLibrary
	{
	public:
		static void Add(const std::string& name, const Ref<Texture>& texture);
		static void Add(const Ref<Texture>& texture);
		static Ref<Texture> Load(const std::string& name, const std::string& filepath);
		static Ref<Texture> Load(const std::string& filepath);

		static Ref<Texture> GetByName(const std::string& name);
		static Ref<Texture> GetByPath(const std::string& path);
		static bool ContainsName(const std::string& name);
		static bool ContainsPath(const std::string& path);

	private:
		static std::unordered_map<std::string, Ref<Texture>> s_Textures;
		static std::unordered_map<std::string, std::string> s_Paths;
	};

}