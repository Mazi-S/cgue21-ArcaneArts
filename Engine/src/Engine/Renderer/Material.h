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
		Ref<UniformBuffer> GetUniformBuffer() { return m_MaterialUB; }

	protected:
		Ref<Shader> m_Shader;
		Ref<UniformBuffer> m_MaterialUB;
	};

}


