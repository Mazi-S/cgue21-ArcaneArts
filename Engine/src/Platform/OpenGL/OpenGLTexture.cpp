#include "egpch.h"
#include "OpenGLTexture.h"

#include <fstream>

#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

namespace Engine {

	Ref<Texture> Texture::Create(const char* filepath)
	{
		return CreateRef<OpenGL::Texture>(filepath);
	}

}

namespace Engine::OpenGL {


	Texture::Texture(const std::string& filepath)
	{
		//m_TextureID = LoadDDS(filepath);
		Load(filepath);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void Texture::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_TextureID);
	}

	bool Texture::Load(const std::string& path)
	{
		stbi_set_flip_vertically_on_load(1);
		int width, height, channels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		ASSERT(data, "Faild to load texture!");

		m_Width = width;
		m_Height = height;
		m_InternalFormat = channels == 3 ? GL_RGB8 : GL_RGBA8;
		m_DataFormat = channels == 3 ? GL_RGB : GL_RGBA;

		glGenTextures(1, &m_TextureID);

		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
		return true;
	}

}