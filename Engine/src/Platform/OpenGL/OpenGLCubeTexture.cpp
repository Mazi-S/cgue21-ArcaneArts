#include "egpch.h"
#include "OpenGLCubeTexture.h"

#include <fstream>

#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

namespace Engine {

	Ref<Texture> Texture::CreateCube(const std::vector<std::string> faces)
	{
		return CreateRef<OpenGL::CubeTexture>(faces);
	}

}

namespace Engine::OpenGL {

	static const std::string NameFromFilepath(const std::string& filepath)
	{
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		return filepath.substr(lastSlash, count);
	}
	CubeTexture::CubeTexture(std::vector<std::string> faces)
		: m_Name(NameFromFilepath(faces[0])), m_Path(faces[0])
	{
		LoadCubemap(faces);
	}

	CubeTexture::~CubeTexture()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void CubeTexture::Bind(uint32_t slot) const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
	}

	bool CubeTexture::LoadCubemap(std::vector<std::string> faces)
	{
		stbi_set_flip_vertically_on_load(1);
		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

		int width, height, channels;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			LOG_TRACE("Loading texture file {} ...", faces[i]);
			unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);

			m_Width = width;
			m_Height = height;
			m_InternalFormat = channels == 3 ? GL_RGB8 : GL_RGBA8;
			m_DataFormat = channels == 3 ? GL_RGB : GL_RGBA;
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data
				);
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

		LOG_TRACE("File loaded successfully!");
		return true;
	}
}