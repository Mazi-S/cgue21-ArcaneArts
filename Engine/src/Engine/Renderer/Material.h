#pragma once
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

#include <stdint.h>
#include <map>
#include <string>
#include <unordered_map>

namespace Engine {

	struct MaterialProperties
	{
		std::string Name;
		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;
		float Shininess;

		std::string Shader;

		std::map<uint16_t, std::string> Textures;

		MaterialProperties(const std::string& name, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess = 1.0f, const std::string& shader = "");
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Material ///////////////////////////////////////////////////////////////////////////////////

	class Material
	{
		friend class MaterialLibrary;
		friend class MaterialPanel;

	public:
		Material(const MaterialProperties& properties, const Ref<OpenGL::GlShader>& shader);
		virtual ~Material() = default;

		Ref<OpenGL::GlShader> GetShader() { return m_Shader; }

		virtual void Bind();
		virtual void Set(const std::string& name, const Ref<OpenGL::GlUniformBuffer>& uniformBuffer);

		const std::string& GetName() const { return m_Name; };

		void SetAmbient(glm::vec3 ambient);
		void SetDiffuse(glm::vec3 diffuse);
		void SetSpecular(glm::vec3 specular);
		void SetShininess(float shininess);

		const glm::vec3& GetAmbient() const { return m_Ambient; };
		const glm::vec3& GetDiffuse() const { return m_Diffuse; };
		const glm::vec3& GetSpecular() const { return m_Specular; };
		float GetShininess() const { return m_Shininess; }

		const std::map<uint16_t, Ref<OpenGL::GlTexture2D>>& GetTextures() const { return m_Textures; }

	protected:
		std::string m_Name;
		uint16_t m_BindingPoint = 0;

		Ref<OpenGL::GlShader> m_Shader;
		Ref<OpenGL::GlUniformBuffer> m_MaterialUB;

		// Properties
		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
		float m_Shininess;

		// Textures
		std::map<uint16_t, Ref<OpenGL::GlTexture2D>> m_Textures;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// TextureMaterial ////////////////////////////////////////////////////////////////////////////

	class TextureMaterial : public Material
	{
	public:
		TextureMaterial(const MaterialProperties& properties, const Ref<OpenGL::GlTexture2D>& colorTex, const Ref<OpenGL::GlShader>& shader);
		
		virtual void Bind() override;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// MaterialLibrary ////////////////////////////////////////////////////////////////////////////

	class MaterialLibrary
	{
		friend class MaterialPanel;

	public:
		static void Load(const std::string& filepath);
		static void Save(const std::string& filepath);

		static Ref<Material> Create(MaterialProperties& properties);

		static Ref<Material> Get(const std::string& name);
		static bool Contains(const std::string& name);

		static bool AddShader(const Ref<OpenGL::GlShader>& shader);
		static bool AddTexture(const Ref<OpenGL::GlTexture2D>& colorTex);

	private:
		static void Add(const Ref<Material>& material);

		static void Serialize(const std::string& filepath);
		static void Deserialize(const std::string& filepath);

	private:
		static std::unordered_map<std::string, Ref<OpenGL::GlShader>> s_Shaders;
		static std::unordered_map<std::string, Ref<OpenGL::GlTexture2D>> s_Textures;

		static std::unordered_map<std::string, Ref<Material>> s_Materials;
	};

}


