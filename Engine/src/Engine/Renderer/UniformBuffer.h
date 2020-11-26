#pragma once
#include "Buffer.h"

namespace Engine {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// UniformBuffer //////////////////////////////////////////////////////////////////////////////

	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() = default;

		virtual void Bind(uint32_t bindingPoint) = 0;
		virtual void SetData(const void* data, std::string name) = 0;

		static Ref<UniformBuffer> Create(uint32_t size, const std::initializer_list<BufferElement>& elements);
	};

}
