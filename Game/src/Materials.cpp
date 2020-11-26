#include "Materials.h"
#include <glm/glm/gtc/type_ptr.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////
// Color Material /////////////////////////////////////////////////////////////////////////////

ColorMaterial::ColorMaterial(const glm::vec4& color)
	: m_Color(color)
{
	if (!Engine::ShaderLibrary::Exists("ColorShader"))
		Engine::ShaderLibrary::Load("ColorShader", "assets/shaders/FlatColor.glsl");

	m_Shader = Engine::ShaderLibrary::Get("ColorShader");

	m_MaterialUB = Engine::UniformBuffer::Create({
		{Engine::ShaderDataType::Float4, "Color"}
	});

	m_MaterialUB->SetData(glm::value_ptr(color), "Color");
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Texture Material ///////////////////////////////////////////////////////////////////////////

TextureMaterial::TextureMaterial(const std::string& filepath)
{
	if (!Engine::ShaderLibrary::Exists("TextureShader"))
		Engine::ShaderLibrary::Load("TextureShader", "assets/shaders/Texture.glsl");

	m_Shader = Engine::ShaderLibrary::Get("TextureShader");
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
