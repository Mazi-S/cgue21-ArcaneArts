#include "egpch.h"
#include "OpenGLTexture.h"

#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

namespace Engine::OpenGL {

	GlTexture2D::GlTexture2D(const std::string& name, const std::string& filepath)
		: m_Name(name), m_Path(filepath)
	{
		Load();
	}

	GlTexture2D::~GlTexture2D()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void GlTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_TextureID);
	}

	bool GlTexture2D::Load()
	{
		LOG_TRACE("Loading 2D texture file {} ...", m_Path);
		stbi_set_flip_vertically_on_load(1);
		int width, height, channels;
		unsigned char* data = stbi_load(m_Path.c_str(), &width, &height, &channels, 0);

		ASSERT(data, "Failed to load texture!");

		m_Width = width;
		m_Height = height;
		m_InternalFormat = channels == 3 ? GL_RGB8 : GL_RGBA8;
		m_DataFormat = channels == 3 ? GL_RGB : GL_RGBA;

		glGenTextures(1, &m_TextureID);

		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
		LOG_TRACE("Texture loaded successfully!");
		return true;
	}
}