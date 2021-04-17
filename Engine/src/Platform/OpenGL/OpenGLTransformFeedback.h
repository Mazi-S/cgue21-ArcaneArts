#pragma once
#include "OpenGLBuffer.h"
#include "Engine/Core/Base.h"

namespace Engine::OpenGL {

	class GlTransformFeedback
	{
	public:
		GlTransformFeedback();
		~GlTransformFeedback();

		void Bind();

		void BindBuffer(const Ref<GlVertexBuffer>& buffer, uint32_t index = 0);
		void BindBuffer(const GlVertexBuffer& buffer, uint32_t index = 0);

		uint32_t GetRendererID() { return m_RendererID; }

	private:
		void BindBuffer(uint32_t bufferID, uint32_t index);

		uint32_t m_RendererID;
	};

}