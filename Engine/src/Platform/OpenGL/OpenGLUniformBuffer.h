#pragma once
#include "OpenGLBufferLayout.h"

namespace Engine::OpenGL {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// UniformBuffer //////////////////////////////////////////////////////////////////////////////

	class GlUniformBuffer
	{
	public:
		GlUniformBuffer() = default;
		GlUniformBuffer(const GlUniformBufferLayout_std140& layout);

		virtual void Bind(uint32_t bindingPoint);
		virtual void SetData(const void* data, std::string name);

	private:
		void SetData(const void* data, uint32_t size, uint32_t offset);
		void SetData(const void* data, uint32_t elemCount, uint32_t elemSize, uint32_t elemOffset, uint32_t offset);

	private:
		uint32_t m_RendererID;
		GlUniformBufferLayout_std140 m_Layout;
	};
}