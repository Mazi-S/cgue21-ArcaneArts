#include "egpch.h"
#include "OpenGLTexture.h"

#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <glad/glad.h>

namespace Engine::OpenGL {
	GlTexture2D::GlTexture2D(const std::string& name, Texture2DSpecification spec)
		: m_Name(name), m_System(true)
	{
		Create(spec);
	}

	GlTexture2D::GlTexture2D(const std::string& name, const std::string& filepath)
		: m_Name(name), m_Path(filepath), m_System(false)
	{
		Texture2DSpecification spec;
		Create(spec, filepath);
	}

	GlTexture2D::GlTexture2D(const std::string& name, Texture2DSpecification spec, const std::string& filepath)
		: m_Name(name), m_Path(filepath), m_System(false)
	{
		Create(spec, filepath);
	}

	GlTexture2D::~GlTexture2D()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void GlTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_TextureID);
	}

	void GlTexture2D::SetData(const void* data, uint32_t size)
	{
		glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Specification.Width, m_Specification.Height, m_Specification.Format, m_Specification.Type, data);
	}

	void GlTexture2D::Reload()
	{
		glDeleteTextures(1, &m_TextureID);
		Create(m_Specification, m_Path);
	}

	void GlTexture2D::Create(const Texture2DSpecification& spec)
	{
		m_Specification = spec;

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		glTexImage2D(GL_TEXTURE_2D, 0, spec.Internalformat, spec.Width, spec.Height, 0, spec.Format, spec.Type, nullptr);

		// Texture Wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, spec.Wrap_S);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, spec.Wrap_T);

		if(spec.Wrap_S == GL_CLAMP_TO_BORDER || spec.Wrap_T == GL_CLAMP_TO_BORDER)
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(spec.Border));

		// Texture Filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, spec.Min_Filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, spec.Mag_Filter);

		if (spec.Mipmaps)
			glGenerateMipmap(GL_TEXTURE_2D);

		if (spec.ComparisonMode)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	}

	void GlTexture2D::Create(Texture2DSpecification& spec, const std::string& filepath)
	{
		LOG_TRACE("Loading 2D texture file {} ...", filepath);
		stbi_set_flip_vertically_on_load(1);
		int width, height, channels;
		unsigned char* data = stbi_load(m_Path.c_str(), &width, &height, &channels, 0);
		ASSERT(data, "Failed to load texture!");

		spec.Width = width;
		spec.Height = height;
		spec.Internalformat = channels == 3 ? GL_RGB8 : GL_RGBA8;
		spec.Format = channels == 3 ? GL_RGB : GL_RGBA;
		m_Specification = spec;

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		glTexImage2D(GL_TEXTURE_2D, 0, spec.Internalformat, spec.Width, spec.Height, 0, spec.Format, GL_UNSIGNED_BYTE, data);

		// Texture Wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, spec.Wrap_S);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, spec.Wrap_T);

		if (spec.Wrap_S == GL_CLAMP_TO_BORDER || spec.Wrap_T == GL_CLAMP_TO_BORDER)
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(spec.Border));

		// Texture Filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, spec.Min_Filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, spec.Mag_Filter);

		if (spec.Mipmaps)
			glGenerateMipmap(GL_TEXTURE_2D);

		if (spec.ComparisonMode)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

		stbi_image_free(data);
		LOG_TRACE("Texture loaded successfully!");
	}
}