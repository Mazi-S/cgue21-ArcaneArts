#pragma once
#include "Buffer.h"

namespace Engine {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Uniform BufferLayout ///////////////////////////////////////////////////////////////////////

	class UniformBufferLayout
	{
		friend class UniformBuffer;

	public:
		UniformBufferLayout() = default;
		UniformBufferLayout(const std::initializer_list<BufferElement>& elements);

		const BufferElement& GetElement(const std::string& name) { return m_Elements[name]; }
		uint32_t Size() const { return m_Size; }

	private:
		std::unordered_map<std::string, BufferElement> m_Elements;
		uint32_t m_Size = 0;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// UniformBuffer //////////////////////////////////////////////////////////////////////////////

	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() = default;

		virtual void Bind(uint32_t bindingPoint) = 0;
		virtual void SetData(const void* data, std::string name) = 0;

		static Ref<UniformBuffer> Create(UniformBufferLayout layout);
	};

}
