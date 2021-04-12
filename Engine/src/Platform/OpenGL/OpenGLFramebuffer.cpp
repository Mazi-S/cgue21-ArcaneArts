#include "egpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

// Temporary: query max. value
static const uint32_t s_MaxFramebufferSize = 8000;

Engine::OpenGL::GlFramebuffer::GlFramebuffer(const FramebufferSpecification& spec)
	: m_Specification(spec)
{
	Init();
}

Engine::OpenGL::GlFramebuffer::~GlFramebuffer()
{
	if (m_RendererID != 0)
		Destroy();
}

void Engine::OpenGL::GlFramebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	glViewport(0, 0, m_Specification.Width, m_Specification.Height);
}

void Engine::OpenGL::GlFramebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Engine::OpenGL::GlFramebuffer::Resize(uint32_t width, uint32_t height)
{
	if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
	{
		LOG_WARN("Attempted to resize frame buffer to {0}, {1}", width, height);
		return;
	}
	m_Specification.Width = width;
	m_Specification.Height = height;

	if (m_RendererID != 0)
		Destroy();

	Init();
}

void Engine::OpenGL::GlFramebuffer::Init()
{
	glCreateFramebuffers(1, &m_RendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

	// color buffer
	if (m_Specification.ColorAttachment)
	{
		Texture2DSpecification colorSpec;
		colorSpec.Width = m_Specification.Width;
		colorSpec.Height = m_Specification.Height;

		colorSpec.Internalformat = m_Specification.ColorAttachment.Internalformat;
		colorSpec.Format = m_Specification.ColorAttachment.Format;
		colorSpec.Type = m_Specification.ColorAttachment.Type;

		colorSpec.Wrap_S = m_Specification.ColorAttachment.Wrap_S;
		colorSpec.Wrap_T = m_Specification.ColorAttachment.Wrap_T;
		colorSpec.Border = m_Specification.ColorAttachment.Border;

		colorSpec.Min_Filter = m_Specification.ColorAttachment.Min_Filter;
		colorSpec.Mag_Filter = m_Specification.ColorAttachment.Mag_Filter;

		colorSpec.Mipmaps = false;

		colorSpec.ComparisonMode = m_Specification.ColorAttachment.ComparisonMode;

		m_DepthAttachment = CreateRef<GlTexture2D>("ColorAttachment", colorSpec);
	}
	else
	{
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	if (m_Specification.DepthAttachment)
	{
		Texture2DSpecification depthSpec;
		depthSpec.Width = m_Specification.Width;
		depthSpec.Height = m_Specification.Height;

		depthSpec.Internalformat = m_Specification.DepthAttachment.Internalformat;
		depthSpec.Format = m_Specification.DepthAttachment.Format;
		depthSpec.Type = m_Specification.DepthAttachment.Type;

		depthSpec.Wrap_S = m_Specification.DepthAttachment.Wrap_S;
		depthSpec.Wrap_T = m_Specification.DepthAttachment.Wrap_T;
		depthSpec.Border = m_Specification.DepthAttachment.Border;

		depthSpec.Min_Filter = m_Specification.DepthAttachment.Min_Filter;
		depthSpec.Mag_Filter = m_Specification.DepthAttachment.Mag_Filter;

		depthSpec.ComparisonMode = m_Specification.DepthAttachment.ComparisonMode;

		depthSpec.Mipmaps = false;

		m_DepthAttachment = CreateRef<GlTexture2D>("DepthAttachment", depthSpec);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment->m_TextureID, 0);
	}

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Frame buffer is incomplete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Engine::OpenGL::GlFramebuffer::Destroy()
{
	glDeleteFramebuffers(1, &m_RendererID);
	m_ColorAttachment = nullptr;
	m_DepthAttachment = nullptr;
}
