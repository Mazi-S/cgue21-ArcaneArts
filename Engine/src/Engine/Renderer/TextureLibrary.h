#pragma once

#include <glm/glm.hpp>
#include "Engine/Core/Base.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/OpenGL/OpenGLCubeTexture.h"

namespace Engine {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Texture 2D Library /////////////////////////////////////////////////////////////////////////

	class Texture2DLibrary
	{
		friend class Texture2DPanel;

	public:
		static void Init();

		static void Load(const std::string& filepath = "assets/Texture2DLibrary.yaml");
		static void Save(const std::string& filepath = "assets/Texture2DLibrary.yaml");

		static Ref<OpenGL::GlTexture2D> Create(const std::string& name, const std::string& filepath);

		static Ref<OpenGL::GlTexture2D> Get(const std::string& name = std::string());
		static std::vector<std::string> GetNames();

		static bool Contains(const std::string& name);

		static void AddSystem(const Ref<OpenGL::GlTexture2D>& texture);
		static void Remove(const std::string& name);
		static void Rename(const std::string oldName, const std::string newName);
	private:
		static void Add(const Ref<OpenGL::GlTexture2D>& texture);

		static void Serialize(const std::string& filepath);
		static void Deserialize(const std::string& filepath);

	private:
		static std::unordered_map<std::string, Ref<OpenGL::GlTexture2D>> s_Textures2D;
		static Ref<OpenGL::GlTexture2D> s_Default;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Cube Texture Library ///////////////////////////////////////////////////////////////////////

	class CubeTextureLibrary
	{
	public:
		static void Add(const Ref<OpenGL::GlCubeTexture>& texture);

		static Ref<OpenGL::GlCubeTexture> LoadCubeTexture(const std::string& name, std::vector<std::string> paths);
		static Ref<OpenGL::GlCubeTexture> GetCubeTexture(const std::string& name);

		static bool ContainsCubeTexture(const std::string& name);

	private:
		static std::unordered_map<std::string, Ref<OpenGL::GlCubeTexture>> s_CubeTextures;
	};


}