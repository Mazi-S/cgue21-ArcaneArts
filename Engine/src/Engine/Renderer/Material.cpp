#include "egpch.h"
#include "Material.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// MaterialProperties /////////////////////////////////////////////////////////////////////////

	MaterialProperties::MaterialProperties(const std::string & name, const glm::vec3 & ambient, const glm::vec3 & diffuse, const glm::vec3 & specular, float shininess)
		: Name(name), Ambient(ambient), Diffuse(diffuse), Specular(specular), Shininess(shininess)
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

	void Material::Set(const std::string& name, const Ref<OpenGL::GlUniformBuffer>& uniformBuffer)
	{
		uniformBuffer->Bind(m_BindingPoint);
		m_Shader->SetBlockBinding(name, m_BindingPoint);
		m_BindingPoint++;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	// TextureMaterial ////////////////////////////////////////////////////////////////////////////

	TextureMaterial::TextureMaterial(const MaterialProperties& properties, const Ref<OpenGL::GlTexture2D>& colorTex, const Ref<OpenGL::GlShader>& shader)
		: Material(properties, shader), m_ColorTexture(colorTex)
	{ }

	void TextureMaterial::Bind()
	{
		Material::Bind();
		m_ColorTexture->Bind();
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// MaterialLibrary ////////////////////////////////////////////////////////////////////////////

	std::unordered_map<std::string, Ref<Material>> MaterialLibrary::s_Materials;

	void MaterialLibrary::Add(const Ref<Material>& material)
	{
		auto& name = material->GetName();
		Add(name, material);
	}

	void MaterialLibrary::Add(const std::string& name, const Ref<Material>& material)
	{
		ASSERT(!Contains(name), "Shader already exists!");
		s_Materials[name] = material;
	}

	Ref<Material> MaterialLibrary::Create(const MaterialProperties& properties, const Ref<OpenGL::GlShader>& shader)
	{
		Ref<Material> material = CreateRef<Material>(properties, shader);
		s_Materials[material->GetName()] = material;
		return material;
	}

	Ref<TextureMaterial> MaterialLibrary::Create(const MaterialProperties& properties, const Ref<OpenGL::GlTexture2D>& colorTex, const Ref<OpenGL::GlShader>& shader)
	{
		Ref<TextureMaterial> material = CreateRef<TextureMaterial>(properties, colorTex, shader);
		s_Materials[material->GetName()] = material;
		return material;
	}

	Ref<Material> MaterialLibrary::Get(const std::string& name)
	{
		ASSERT(Contains(name), "Material not found!");
		return s_Materials[name];
	}

	bool MaterialLibrary::Contains(const std::string& name)
	{
		return s_Materials.find(name) != s_Materials.end();
	}


}

