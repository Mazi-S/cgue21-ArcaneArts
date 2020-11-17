#pragma once

#include "Engine/Renderer/UniformBuffer.h"

namespace Engine::OpenGL {

	class UniformBuffer : public Engine::UniformBuffer
	{
	public:
		UniformBuffer() = default;
		UniformBuffer(const UniformBufferLayout& layout);

		virtual void Bind(uint32_t bindingPoint) override;
		virtual void SetData(const void* data, std::string name) override;

	private:
		void SetData(const void* data, uint32_t size, uint32_t offset);
		void SetData(const void* data, uint32_t elemCount, uint32_t elemSize, uint32_t elemOffset, uint32_t offset);

	private:
		uint32_t m_RendererID;
		UniformBufferLayout m_Layout;
	};

}