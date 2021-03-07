#include "egpch.h"
#include "TextureLibrary.h"

namespace Engine {

	std::unordered_map<std::string, Ref<OpenGL::GlTexture2D>> TextureLibrary::s_Textures2D;
	std::unordered_map<std::string, Ref<OpenGL::GlCubeTexture>> TextureLibrary::s_CubeTextures;

	void TextureLibrary::Add(const Ref<OpenGL::GlTexture2D>& texture)
	{
		auto& name = texture->GetName();
		Add(name, texture);
	}

	void TextureLibrary::Add(const std::string& name, const Ref<OpenGL::GlTexture2D>& texture)
	{
		ASSERT(!ContainsTexture2D(name), "Texture already exists!");
		s_Textures2D[name] = texture;
	}

	void TextureLibrary::Add(const Ref<OpenGL::GlCubeTexture>& texture)
	{
		auto& name = texture->GetName();
		Add(name, texture);
	}
	
	void TextureLibrary::Add(const std::string& name, const Ref<OpenGL::GlCubeTexture>& texture)
	{
		ASSERT(!ContainsCubeTexture(name), "Texture already exists!");
		s_CubeTextures[name] = texture;
	}
	
	Ref<OpenGL::GlTexture2D> TextureLibrary::LoadTexture2D(const std::string& name, const std::string& filepath)
	{
		auto texture = CreateRef<OpenGL::GlTexture2D>(name, filepath);
		Add(name, texture);
		return texture;
	}

	Ref<OpenGL::GlTexture2D> TextureLibrary::GetTexture2D(const std::string& name)
	{
		ASSERT(ContainsTexture2D(name), "Texture not found!");
		return s_Textures2D[name];
	}

	bool TextureLibrary::ContainsTexture2D(const std::string& name)
	{
		return s_Textures2D.find(name) != s_Textures2D.end();
	}
	
	Ref<OpenGL::GlCubeTexture> TextureLibrary::LoadCubeTexture(const std::string& name, std::vector<std::string> paths)
	{
		auto texture = CreateRef<OpenGL::GlCubeTexture>(name, paths);
		Add(name, texture);
		return texture;
	}
	
	Ref<OpenGL::GlCubeTexture> TextureLibrary::GetCubeTexture(const std::string& name)
	{
		ASSERT(ContainsCubeTexture(name), "Texture not found!");
		return s_CubeTextures[name];
	}
	
	bool TextureLibrary::ContainsCubeTexture(const std::string& name)
	{
		return s_CubeTextures.find(name) != s_CubeTextures.end();
	}
}
