#pragma once

#include "Shader.h"
#include "UniformBuffer.h"

namespace Engine {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Material ///////////////////////////////////////////////////////////////////////////////////

	class Material
	{
	public:
		virtual ~Material() = default;

		Ref<Shader> GetShader() { return m_Shader; }

		virtual void Bind();
		virtual void Set(const std::string& name, const Ref<UniformBuffer>& uniformBuffer);

	protected:
		uint16_t m_BindingPoint;

		Ref<Shader> m_Shader;
		Ref<UniformBuffer> m_MaterialUB;
	};

}


