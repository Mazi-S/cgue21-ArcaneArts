#include "egpch.h"
#include "Material.h"
#include <glm/gtc/type_ptr.hpp>
#include "ShaderLibrary.h"
#include "TextureLibrary.h"

namespace Engine {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// MaterialProperties /////////////////////////////////////////////////////////////////////////

	MaterialProperties::MaterialProperties(const std::string & name, const glm::vec3 & ambient, const glm::vec3 & diffuse, const glm::vec3 & specular, float shininess, const std::string& shader)
		: Name(name), Ambient(ambient), Diffuse(diffuse), Specular(specular), Shininess(shininess), Shader(shader)
	{ }

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Material ///////////////////////////////////////////////////////////////////////////////////

	Material::Material(const MaterialProperties& prop)
		: m_Name(prop.Name), m_Ambient(prop.Ambient), m_Diffuse(prop.Diffuse), m_Specular(prop.Specular), m_Shininess(prop.Shininess), m_Shader(prop.Shader)
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

		for (const auto& entry : prop.Textures)
			m_Textures[entry.first] = entry.second;
	}

	void Material::Bind()
	{
		ShaderLibrary::Get(m_Shader)->Bind();
		m_MaterialUB->Bind(2);

		for (auto& entry : m_Textures)
			Texture2DLibrary::Get(entry.second)->Bind(entry.first);
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

}

