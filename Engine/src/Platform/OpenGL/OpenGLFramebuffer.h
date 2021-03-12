#pragma once

#include "Engine/Core/Base.h"
#include "OpenGLTexture.h"

#include <glm/glm.hpp>

namespace Engine::OpenGL {

	// Framebuffers
	//	|-> color buffer
	//	|-> depth buffer
	//	|-> stencil buffer

	struct FramebufferAttachmentSpecification
	{
		bool Attach = false;

		uint32_t Internalformat;
		uint32_t Format;
		uint32_t Type;

		// Texture Wrapping
		uint32_t Wrap_S = 0x2901;
		uint32_t Wrap_T = 0x2901;
		glm::vec4 Border;

		// Texture Filtering
		uint32_t Min_Filter = 0x2600;
		uint32_t Mag_Filter = 0x2600;

		operator bool() const { return Attach; }
	};

	struct FramebufferSpecification
	{
		uint32_t Width = 0, Height = 0;

		FramebufferAttachmentSpecification ColorAttachment;
		FramebufferAttachmentSpecification DepthAttachment;

		// FramebufferFormat Format = 
		// uint32_t Samples = 1;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Frame Buffer ///////////////////////////////////////////////////////////////////////////////

	class GlFramebuffer
	{
	public:
		GlFramebuffer(const FramebufferSpecification& spec);
		virtual ~GlFramebuffer();

		virtual void Bind();
		virtual void Unbind();

		virtual void Resize(uint32_t width, uint32_t height);

		virtual Ref<GlTexture2D> GetColorAttachment() const { return m_ColorAttachment; }
		virtual Ref<GlTexture2D> GetDepthAttachment() const { return m_DepthAttachment; }

		virtual const FramebufferSpecification& GetSpecification() const { return m_Specification; }

	private:
		void Init();
		void Destroy();

	private:
		uint32_t m_RendererID = 0;
		Ref<GlTexture2D> m_ColorAttachment;
		Ref<GlTexture2D> m_DepthAttachment;
		FramebufferSpecification m_Specification;
	};

}