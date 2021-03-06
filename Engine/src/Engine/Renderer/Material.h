#pragma once

#include "Shader.h"
#include "Texture.h"

#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Engine {

	struct MaterialProperties
	{
		std::string Name;
		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;
		float Shininess;

		// Textures
		std::string ColorTex_path;

		MaterialProperties(const std::string& name, const glm::vec3& color, float shininess = 1.0f);
		MaterialProperties(const std::string& name, const glm::vec3& color, const std::string& colorTex_path, float shininess = 1.0f);
		MaterialProperties(const std::string& name, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess = 1.0f, const std::string& colorTex_path = "");
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Material ///////////////////////////////////////////////////////////////////////////////////

	class Material
	{
	public:
		Material(const std::string& name, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess, const Ref<Shader>& shader);
		virtual ~Material() = default;

		Ref<Shader> GetShader() { return m_Shader; }

		virtual void Bind();
		virtual void Set(const std::string& name, const Ref<OpenGL::GlUniformBuffer>& uniformBuffer);

		const std::string& GetName() const { return m_Name; };

		static Ref<Material> Create(const MaterialProperties& properties, const Ref<Shader>& shader);

	protected:
		std::string m_Name;
		uint16_t m_BindingPoint = 0;

		Ref<Shader> m_Shader;
		Ref<OpenGL::GlUniformBuffer> m_MaterialUB;

		// Properties
		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
		float m_Shininess;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// TextureMaterial ////////////////////////////////////////////////////////////////////////////

	class TextureMaterial : public Material
	{
	public:
		TextureMaterial(const std::string& name, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess, const Ref<Texture>& colorTex, const Ref<Shader>& shader);
		
		virtual void Bind() override;
	
	private:
		Ref<Texture> m_ColorTexture;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// MaterialLibrary ////////////////////////////////////////////////////////////////////////////

	class MaterialLibrary
	{
	public:
		static void Add(const Ref<Material>& material);
		static void Add(const std::vector<Ref<Material>>& materials);

		static Ref<Material> Get(const std::string& name);
		static bool Exists(const std::string& name);

	private:
		static std::unordered_map<std::string, Ref<Material>> s_Materials;
	};

}


