#pragma once

#include "Engine/Renderer/UniformBuffer.h"

namespace Engine::OpenGL {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// BufferLayout - std140 //////////////////////////////////////////////////////////////////////

	class BufferLayout_std140
	{
	public:
		BufferLayout_std140() = default;
		BufferLayout_std140(uint32_t size, const std::initializer_list<BufferElement>& elements);

		const BufferElement& GetElement(const std::string& name) { return m_Elements[name]; }
		uint32_t Size() const { return m_Size; }

	private:
		std::unordered_map<std::string, BufferElement> m_Elements;
		uint32_t m_Size = 0;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// UniformBuffer //////////////////////////////////////////////////////////////////////////////

	class UniformBuffer : public Engine::UniformBuffer
	{
	public:
		UniformBuffer() = default;
		UniformBuffer(const BufferLayout_std140& layout);

		virtual void Bind(uint32_t bindingPoint) override;
		virtual void SetData(const void* data, std::string name) override;

	private:
		void SetData(const void* data, uint32_t size, uint32_t offset);
		void SetData(const void* data, uint32_t elemCount, uint32_t elemSize, uint32_t elemOffset, uint32_t offset);

	private:
		uint32_t m_RendererID;
		BufferLayout_std140 m_Layout;
	};

}