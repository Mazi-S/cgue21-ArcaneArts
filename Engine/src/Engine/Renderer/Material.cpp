#include "egpch.h"
#include "Material.h"

namespace Engine {

	void Material::Bind()
	{
		m_BindingPoint = 0;
		m_Shader->Bind();
		Set("MaterialData", m_MaterialUB);
	}

	void Material::Set(const std::string& name, const Ref<UniformBuffer>& uniformBuffer)
	{
		uniformBuffer->Bind(m_BindingPoint);
		m_Shader->SetBlockBinding(name, m_BindingPoint);
		m_BindingPoint++;
	}

}

