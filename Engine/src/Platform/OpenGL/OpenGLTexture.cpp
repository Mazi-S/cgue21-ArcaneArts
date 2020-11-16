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

	std::uint32_t Texture::LoadDDS(const char* filepath)
	{
		unsigned char header[124];

		FILE* fp;

		/* try to open the file */
		fp = fopen(filepath, "rb");
		if (fp == NULL) {
			// todo: remove use logging lib. => e.g. LOG_INFO(msg)
			printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", filepath); getchar();
			return 0;
		}

		/* verify the type of file */
		char filecode[4];
		fread(filecode, 1, 4, fp);
		if (strncmp(filecode, "DDS ", 4) != 0) {
			fclose(fp);
			return 0;
		}

		/* get the surface desc */
		fread(&header, 124, 1, fp);

		unsigned int height = *(unsigned int*)&(header[8]);
		unsigned int width = *(unsigned int*)&(header[12]);
		unsigned int linearSize = *(unsigned int*)&(header[16]);
		unsigned int mipMapCount = *(unsigned int*)&(header[24]);
		unsigned int fourCC = *(unsigned int*)&(header[80]);


		unsigned char* buffer;
		unsigned int bufsize;
		/* how big is it going to be including all mipmaps? */
		bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
		buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
		fread(buffer, 1, bufsize, fp);
		/* close the file pointer */
		fclose(fp);

		unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
		unsigned int format;
		switch (fourCC)
		{
		case FOURCC_DXT1:
			format = GL_VIEW_CLASS_S3TC_DXT1_RGBA;
			break;
		case FOURCC_DXT3:
			format = GL_VIEW_CLASS_S3TC_DXT3_RGBA;
			break;
		case FOURCC_DXT5:
			format = GL_VIEW_CLASS_S3TC_DXT5_RGBA;
			break;
		default:
			free(buffer);
			return 0;
		}

		// Create one OpenGL texture
		std::uint32_t textureID;
		glGenTextures(1, &textureID);

		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		unsigned int blockSize = (format == GL_VIEW_CLASS_S3TC_DXT1_RGBA) ? 8 : 16;
		unsigned int offset = 0;

		/* load the mipmaps */
		for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
		{
			unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
			glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
				0, size, buffer + offset);

			offset += size;
			width /= 2;
			height /= 2;

			// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
			if (width < 1) width = 1;
			if (height < 1) height = 1;

		}

		free(buffer);

		return textureID;
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