#pragma once

#include "glm/glm.hpp"

typedef unsigned int GLenum;

namespace Engine::OpenGL {

	class GlShader
	{
	public:
		GlShader(const std::string& name, const std::string& filepath, bool link = true);
		virtual ~GlShader();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual void SetBlockBinding(const std::string& name, uint32_t bindingPoint);
		virtual void SetTransformFeedbackVaryings(uint32_t count, char* varyings[], uint32_t bufferMode);

		virtual void SetInt(const std::string& name, int value);
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count);
		virtual void SetFloat(const std::string& name, float value);
		virtual void SetFloat2(const std::string& name, const glm::vec2& values);
		virtual void SetFloat3(const std::string& name, const glm::vec3& values);
		virtual void SetFloat4(const std::string& name, const glm::vec4& values);
		virtual void SetMat3(const std::string& name, const glm::mat3& matrix);
		virtual void SetMat4(const std::string& name, const glm::mat4& matrix);

		virtual const std::string& GetName() const { return m_Name; }
		virtual const std::string& GetPath() const { return m_Path; }

		void Link();
	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string> shaderSources);

	private:
		std::vector<GLenum> m_ShaderIDs;

		const std::string m_Name;
		const std::string m_Path;
		uint32_t m_RendererID;
	};

}
