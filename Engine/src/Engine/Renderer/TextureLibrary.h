#pragma once

#include <glm/glm.hpp>
#include "Engine/Core/Base.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/OpenGL/OpenGLCubeTexture.h"

namespace Engine {

	class TextureLibrary
	{
	public:
		// Texture 2D
		static void Add(const Ref<OpenGL::GlTexture2D>& texture);
		static void Add(const std::string& name, const Ref<OpenGL::GlTexture2D>& texture);

		static Ref<OpenGL::GlTexture2D> LoadTexture2D(const std::string& name, const std::string& filepath);
		static Ref<OpenGL::GlTexture2D> GetTexture2D(const std::string& name);

		static bool ContainsTexture2D(const std::string& name);

		// Cube Texture
		static void Add(const Ref<OpenGL::GlCubeTexture>& texture);
		static void Add(const std::string& name, const Ref<OpenGL::GlCubeTexture>& texture);

		static Ref<OpenGL::GlCubeTexture> LoadCubeTexture(const std::string& name, std::vector<std::string> paths);
		static Ref<OpenGL::GlCubeTexture> GetCubeTexture(const std::string& name);

		static bool ContainsCubeTexture(const std::string& name);

	private:
		static std::unordered_map<std::string, Ref<OpenGL::GlTexture2D>> s_Textures2D;
		static std::unordered_map<std::string, Ref<OpenGL::GlCubeTexture>> s_CubeTextures;
	};

}