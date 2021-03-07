#pragma once

#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Engine {

	struct MaterialProperties
	{
		std::string Name;
		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;
		float Shininess;

		MaterialProperties(const std::string& name, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess = 1.0f);
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Material ///////////////////////////////////////////////////////////////////////////////////

	class Material
	{
	public:
		Material(const MaterialProperties& properties, const Ref<OpenGL::GlShader>& shader);
		virtual ~Material() = default;

		Ref<OpenGL::GlShader> GetShader() { return m_Shader; }

		virtual void Bind();
		virtual void Set(const std::string& name, const Ref<OpenGL::GlUniformBuffer>& uniformBuffer);

		const std::string& GetName() const { return m_Name; };

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
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// TextureMaterial ////////////////////////////////////////////////////////////////////////////

	class TextureMaterial : public Material
	{
	public:
		TextureMaterial(const MaterialProperties& properties, const Ref<OpenGL::GlTexture2D>& colorTex, const Ref<OpenGL::GlShader>& shader);
		
		virtual void Bind() override;
	
	private:
		Ref<OpenGL::GlTexture2D> m_ColorTexture;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// MaterialLibrary ////////////////////////////////////////////////////////////////////////////

	class MaterialLibrary
	{
	public:
		static void Add(const Ref<Material>& material);
		static void Add(const std::string& name, const Ref<Material>& material);
		static Ref<Material> Create(const MaterialProperties& properties, const Ref<OpenGL::GlShader>& shader);
		static Ref<TextureMaterial> Create(const MaterialProperties& properties, const Ref<OpenGL::GlTexture2D>& colorTex, const Ref<OpenGL::GlShader>& shader);

		static Ref<Material> Get(const std::string& name);
		static bool Contains(const std::string& name);

	private:
		static std::unordered_map<std::string, Ref<Material>> s_Materials;
	};

}


