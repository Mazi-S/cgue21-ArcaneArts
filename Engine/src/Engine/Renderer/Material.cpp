#include "egpch.h"
#include "Material.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// MaterialProperties /////////////////////////////////////////////////////////////////////////

	MaterialProperties::MaterialProperties(const std::string& name, const glm::vec3& color, float shininess)
		: MaterialProperties(name, color, color, color, shininess)
	{ }

	MaterialProperties::MaterialProperties(const std::string & name, const glm::vec3 & color, const std::string & colorTex_path, float shininess)
		: MaterialProperties(name, color, color, color, shininess, colorTex_path)
	{ }

	MaterialProperties::MaterialProperties(const std::string & name, const glm::vec3 & ambient, const glm::vec3 & diffuse, const glm::vec3 & specular, float shininess, const std::string & colorTex_path)
		: Name(name), Ambient(ambient), Diffuse(diffuse), Specular(specular), Shininess(shininess), ColorTex_path(colorTex_path)
	{ }

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Material ///////////////////////////////////////////////////////////////////////////////////

	Material::Material(const std::string& name, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess, const Ref<Shader>& shader)
		: m_Name(name), m_Ambient(ambient), m_Diffuse(diffuse), m_Specular(specular), m_Shininess(shininess), m_Shader(shader)
	{
		m_MaterialUB = Engine::UniformBuffer::Create(4 * 4 * 3, {
			{ShaderDataType::Float3, "Ambient", 0},
			{ShaderDataType::Float3, "Diffuse", 4 * 4},
			{ShaderDataType::Float3, "Specular", 4 * 4 * 2},
			{ShaderDataType::Float, "Shininess", 4 * 4 * 2 + 4 * 3},
		});

		m_MaterialUB->SetData(glm::value_ptr(m_Ambient), "Ambient");
		m_MaterialUB->SetData(glm::value_ptr(m_Diffuse), "Diffuse");
		m_MaterialUB->SetData(glm::value_ptr(m_Specular), "Specular");
		m_MaterialUB->SetData(&m_Shininess, "Shininess");
	}

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

	Ref<Material> Material::Create(const MaterialProperties& properties, const Ref<Shader>& shader)
	{
		if (properties.ColorTex_path != "")
		{
			// todo: Use TextureLibrary
			Ref<Texture> colorTex = Texture::Create(properties.ColorTex_path);
			return CreateRef<TextureMaterial>(properties.Name, properties.Ambient, properties.Diffuse, properties.Specular, properties.Shininess, colorTex,shader);
		}

		return CreateRef<Material>(properties.Name, properties.Ambient, properties.Diffuse, properties.Specular, properties.Shininess, shader);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	// TextureMaterial ////////////////////////////////////////////////////////////////////////////

	TextureMaterial::TextureMaterial(const std::string& name, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess, const Ref<Texture>& colorTex, const Ref<Shader>& shader)
		: Material(name, ambient, diffuse, specular, shininess, shader), m_ColorTexture(colorTex)
	{ }

	void TextureMaterial::Bind()
	{
		Engine::Material::Bind();
		m_ColorTexture->Bind();
	}

}

