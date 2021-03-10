#pragma once

namespace Engine::OpenGL {

	struct GlFramebufferSpecification
	{
		uint32_t Width = 0, Height = 0;
		// FramebufferFormat Format = 
		// uint32_t Samples = 1;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Frame Buffer ///////////////////////////////////////////////////////////////////////////////

	class GlFramebuffer
	{
	public:
		GlFramebuffer(const GlFramebufferSpecification& spec);
		virtual ~GlFramebuffer();

		virtual void Bind();
		virtual void Unbind();

		virtual void Resize(uint32_t width, uint32_t height);

		virtual uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; }
		virtual uint32_t GetDepthAttachmentRendererID() const { return m_DepthAttachment; }

		virtual const GlFramebufferSpecification& GetSpecification() const { return m_Specification; }

	private:
		void Init();
		void Destroy();

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
		GlFramebufferSpecification m_Specification;
	};

}