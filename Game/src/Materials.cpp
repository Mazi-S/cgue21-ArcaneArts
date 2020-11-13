#include "Materials.h"
#include <glm/glm/gtc/type_ptr.hpp>

static Engine::Ref<Engine::ShaderLibrary> s_ShaderLibrary = Engine::CreateRef<Engine::ShaderLibrary>();

ColorMaterial::ColorMaterial(const glm::vec4& color)
	: m_Color(color)
{
	if (!s_ShaderLibrary->Exists("ColorShader"))
		s_ShaderLibrary->Load("ColorShader", "assets/shaders/FlatColor.glsl");

	m_Shader = s_ShaderLibrary->Get("ColorShader");

	m_MaterialUB = Engine::UniformBuffer::Create({
		{Engine::ShaderDataType::Float4, "Color"}
	});

	m_MaterialUB->SetData(glm::value_ptr(color), "Color");
}
