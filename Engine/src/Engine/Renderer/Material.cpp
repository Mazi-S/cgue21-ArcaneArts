#include "egpch.h"
#include "Material.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// MaterialProperties /////////////////////////////////////////////////////////////////////////

	MaterialProperties::MaterialProperties(const std::string & name, const glm::vec3 & ambient, const glm::vec3 & diffuse, const glm::vec3 & specular, float shininess, const std::string& shader)
		: Name(name), Ambient(ambient), Diffuse(diffuse), Specular(specular), Shininess(shininess), Shader(shader)
	{ }

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Material ///////////////////////////////////////////////////////////////////////////////////

	Material::Material(const MaterialProperties& prop, const Ref<OpenGL::GlShader>& shader)
		: m_Name(prop.Name), m_Ambient(prop.Ambient), m_Diffuse(prop.Diffuse), m_Specular(prop.Specular), m_Shininess(prop.Shininess), m_Shader(shader)
	{
		OpenGL::GlUniformBufferLayout_std140 layout(4 * 4 * 3, {
			{OpenGL::GlShaderDataType::Float3, "Ambient", 0},
			{OpenGL::GlShaderDataType::Float3, "Diffuse", 4 * 4},
			{OpenGL::GlShaderDataType::Float3, "Specular", 4 * 4 * 2},
			{OpenGL::GlShaderDataType::Float, "Shininess", 4 * 4 * 2 + 4 * 3},
		});
		m_MaterialUB = CreateRef<OpenGL::GlUniformBuffer>(layout);

		SetAmbient(m_Ambient);
		SetDiffuse(m_Diffuse);
		SetSpecular(m_Specular);
		SetShininess(m_Shininess);
	}

	void Material::Bind()
	{
		m_BindingPoint = 0;
		m_Shader->Bind();
		Set("MaterialData", m_MaterialUB);
	}

	void Material::Set(const std::string& name, const Ref<OpenGL::GlUniformBuffer>& uniformBuffer)
	{
		uniformBuffer->Bind(m_BindingPoint);
		m_Shader->SetBlockBinding(name, m_BindingPoint);
		m_BindingPoint++;
	}

	void Material::SetAmbient(glm::vec3 ambient)
	{
		m_Ambient = ambient;
		m_MaterialUB->SetData(glm::value_ptr(ambient), "Ambient");
	}

	void Material::SetDiffuse(glm::vec3 diffuse)
	{
		m_Diffuse = diffuse;
		m_MaterialUB->SetData(glm::value_ptr(diffuse), "Diffuse");
	}

	void Material::SetSpecular(glm::vec3 specular)
	{
		m_Specular = specular;
		m_MaterialUB->SetData(glm::value_ptr(specular), "Specular");
	}

	void Material::SetShininess(float shininess)
	{
		m_Shininess = shininess;
		m_MaterialUB->SetData(&shininess, "Shininess");
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	// TextureMaterial ////////////////////////////////////////////////////////////////////////////

	TextureMaterial::TextureMaterial(const MaterialProperties& properties, const Ref<OpenGL::GlTexture2D>& colorTex, const Ref<OpenGL::GlShader>& shader)
		: Material(properties, shader)
	{
		// ColorTexture = slot 0
		m_Textures[0] = colorTex;

	}

	void TextureMaterial::Bind()
	{
		Material::Bind();

		for(auto& entry : m_Textures)
			entry.second->Bind(entry.first);
	}

}

