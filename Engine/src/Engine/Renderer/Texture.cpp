#include "egpch.h"
#include "Texture.h"

namespace Engine {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// TextureLibrary /////////////////////////////////////////////////////////////////////////////

	std::unordered_map<std::string, Ref<Texture>> TextureLibrary::s_Textures;
	std::unordered_map<std::string, std::string> TextureLibrary::s_Paths;

	void TextureLibrary::Add(const Ref<Texture>& texture)
	{
		auto& name = texture->GetName();
		Add(name, texture);
	}

	void TextureLibrary::Add(const std::string& name, const Ref<Texture>& texture)
	{
		ASSERT(!ContainsName(name), "Texture already exists!");
		s_Textures[name] = texture;
		s_Paths[texture->GetPath()] = name;
	}

	Ref<Texture> TextureLibrary::Load(const std::string& filepath)
	{
		auto texture = Texture::Create(filepath);
		Add(texture);
		return texture;
	}

	Ref<Texture> TextureLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto texture = Texture::Create(filepath);
		Add(name, texture);
		return texture;
	}

	Ref<Texture> TextureLibrary::GetByName(const std::string& name)
	{
		ASSERT(ContainsName(name), "Texture not found!");
		return s_Textures[name];
	}

	Ref<Texture> TextureLibrary::GetByPath(const std::string& path)
	{
		ASSERT(ContainsPath(path), "Texture not found!");
		return s_Textures[s_Paths[path]];
	}

	bool TextureLibrary::ContainsName(const std::string& name)
	{
		return s_Textures.find(name) != s_Textures.end();
	}

	bool TextureLibrary::ContainsPath(const std::string& path)
	{
		return s_Paths.find(path) != s_Paths.end();
	}

}
