#include "Materials.h"
#include <glm/glm/gtc/type_ptr.hpp>

static Engine::Ref<Engine::ShaderLibrary> s_ShaderLibrary = Engine::CreateRef<Engine::ShaderLibrary>();

///////////////////////////////////////////////////////////////////////////////////////////////
// Color Material /////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////////
// Texture Material ///////////////////////////////////////////////////////////////////////////

TextureMaterial::TextureMaterial(const std::string& filepath)
{
	if (!s_ShaderLibrary->Exists("TextureShader"))
		s_ShaderLibrary->Load("TextureShader", "assets/shaders/Texture.glsl");

	m_Shader = s_ShaderLibrary->Get("TextureShader");
	m_Texture = Engine::Texture::Create(filepath);

	m_MaterialUB = Engine::UniformBuffer::Create({
		{Engine::ShaderDataType::Float4, "Color"}
	});
	
	
	glm::vec4 color(1.0f);
	m_MaterialUB->SetData(glm::value_ptr(color), "Color");
}

void TextureMaterial::Bind()
{
	Engine::Material::Bind();
	m_Texture->Bind();
}
