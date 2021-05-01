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
		static void Load(const std::string& filepath);
		static void Save(const std::string& filepath);

		static void Add(const Ref<OpenGL::GlTexture2D>& texture);

		static Ref<OpenGL::GlTexture2D> LoadTexture2D(const std::string& name, const std::string& filepath);
		static Ref<OpenGL::GlTexture2D> GetTexture2D(const std::string& name);

		static bool ContainsTexture2D(const std::string& name);

	private:
		static std::unordered_map<std::string, Ref<OpenGL::GlTexture2D>> s_Textures2D;

		static void Serialize(const std::string& filepath);
		static void Deserialize(const std::string& filepath);
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