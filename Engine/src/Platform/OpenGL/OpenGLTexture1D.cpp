#include "egpch.h"
#include "OpenGLTexture1D.h"
#include <glad/glad.h>

#include "Engine/Renderer/ParticleSystem.h"

namespace Engine::OpenGL {

	GlTexture1D::GlTexture1D(const std::string& name, Texture1DSpecification spec)
		: m_Name(name)
	{
		Create(spec);
	}

	GlTexture1D::~GlTexture1D()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void GlTexture1D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_TextureID);
	}

	void GlTexture1D::SetData(const void* data, uint32_t size)
	{
		glTextureSubImage1D(m_TextureID, 0, 0, m_Specification.Width, m_Specification.Format, GL_UNSIGNED_BYTE, data);
	}

	void GlTexture1D::Create(const Texture1DSpecification& spec)
	{
		m_Specification = spec;

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_1D, m_TextureID);

		glTexImage1D(GL_TEXTURE_1D, 0, spec.Internalformat, spec.Width, 0, spec.Format, spec.Type, nullptr);

		// Texture Filtering
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, spec.Min_Filter);
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, spec.Mag_Filter);

		// Texture Wrapping
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, spec.Wrap_S);
	}

}
