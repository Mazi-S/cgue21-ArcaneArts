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

		MaterialProperties(const std::string& name,
			const glm::vec3& ambient = { 1.0, 1.0, 1.0 },
			const glm::vec3& diffuse = { 0.8, 0.8, 0.8 },
			const glm::vec3& specular = { 0.5, 0.5, 0.5 },
			float shininess = 1.0f,
			const std::string& shader = "DefaultShader");
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Material ///////////////////////////////////////////////////////////////////////////////////

	class Material
	{
		friend class MaterialLibrary;
		friend class MaterialPanel;

	public:
		Material(const MaterialProperties& properties);
		virtual ~Material() = default;

		std::string GetShader() { return m_Shader; }
		void SetShader(std::string shader) { m_Shader = shader; }

		void SetTexture(uint16_t slot, std::string texture = "DefaultTexture") { m_Textures[slot] = texture; }
		const std::map<uint16_t, std::string>& GetTextures() const { return m_Textures; }
		void RemoveTexture(uint16_t slot) { m_Textures.erase(slot); }

		virtual void Bind();

		const std::string& GetName() const { return m_Name; };

		void SetAmbient(glm::vec3 ambient);
		void SetDiffuse(glm::vec3 diffuse);
		void SetSpecular(glm::vec3 specular);
		void SetShininess(float shininess);

		const glm::vec3& GetAmbient() const { return m_Ambient; };
		const glm::vec3& GetDiffuse() const { return m_Diffuse; };
		const glm::vec3& GetSpecular() const { return m_Specular; };
		float GetShininess() const { return m_Shininess; }

	protected:
		std::string m_Name;
		uint16_t m_BindingPoint = 0;

		std::string m_Shader;
		Ref<OpenGL::GlUniformBuffer> m_MaterialUB;

		// Properties
		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
		float m_Shininess;

		// Textures
		std::map<uint16_t, std::string> m_Textures;
	};

}


