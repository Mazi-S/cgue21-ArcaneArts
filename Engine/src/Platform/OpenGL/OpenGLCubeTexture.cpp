#include "egpch.h"
#include "OpenGLCubeTexture.h"

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <stb_image.h>

namespace Engine::OpenGL {

	GlCubeTexture::GlCubeTexture(const std::string& name, std::vector<std::string> paths)
		: m_Name(name), m_Paths(paths)
	{
		LoadCubemap();
	}

	GlCubeTexture::~GlCubeTexture()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void GlCubeTexture::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_TextureID);
	}

	bool GlCubeTexture::LoadCubemap()
	{
		stbi_set_flip_vertically_on_load(1);
		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

		int width, height, channels;
		for (unsigned int i = 0; i < m_Paths.size(); i++)
		{
			LOG_TRACE("Loading texture file {} ...", m_Paths[i]);
			unsigned char* data = stbi_load(m_Paths[i].c_str(), &width, &height, &channels, 0);

			m_Width = width;
			m_Height = height;
			m_InternalFormat = channels == 3 ? GL_RGB8 : GL_RGBA8;
			m_DataFormat = channels == 3 ? GL_RGB : GL_RGBA;
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				ASSERT(data, "Failed to load texture!");
				stbi_image_free(data);
			}
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		LOG_TRACE("Files loaded successfully!");

		return true;
	}
}