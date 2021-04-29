#include "egpch.h"
#include "OpenGLShader.h"

#include <fstream>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Engine::OpenGL {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "geometry")
			return GL_GEOMETRY_SHADER;
		if (type == "fragment")
			return GL_FRAGMENT_SHADER;

		ASSERT(false, "Unknown shader type: {0}", type);
		return 0;
	}

	GlShader::GlShader(const std::string& name, const std::string& filepath, bool link)
		: m_Name(name), m_Path(filepath)
	{
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);
		if (link)
			Link();
	}

	GlShader::~GlShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void GlShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void GlShader::Unbind() const
	{
		glUseProgram(0);
	}

	void GlShader::SetBlockBinding(const std::string& name, uint32_t bindingPoint)
	{
		GLint blockIndex = glGetUniformBlockIndex(m_RendererID, name.c_str());
		ASSERT(blockIndex != GL_INVALID_INDEX, "'" + name + "' does not correspond to an active uniform block in this shader!", name);
		glUniformBlockBinding(m_RendererID, blockIndex, bindingPoint);
	}

	void GlShader::SetTransformFeedbackVaryings(uint32_t count, char* varyings[], uint32_t bufferMode)
	{
		glTransformFeedbackVaryings(m_RendererID, 4, varyings, bufferMode);
	}

	void GlShader::SetInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		ASSERT(location != -1, "'" + name + "' does not correspond to an active uniform variable in this shader!", name);
		glUniform1i(location, value);
	}

	void GlShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		ASSERT(location != -1, "'" + name + "' does not correspond to an active uniform variable in this shader!", name);
		glUniform1iv(location, count, values);
	}

	void GlShader::SetFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		ASSERT(location != -1, "'" + name + "' does not correspond to an active uniform variable in this shader!", name);
		glUniform1f(location, value);
	}

	void GlShader::SetFloat2(const std::string& name, const glm::vec2& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		ASSERT(location != -1, "'" + name + "' does not correspond to an active uniform variable in this shader!", name);
		glUniform2fv(location, 1, glm::value_ptr(values));
	}

	void GlShader::SetFloat3(const std::string& name, const glm::vec3& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		ASSERT(location != -1, "'" + name + "' does not correspond to an active uniform variable in this shader!", name);
		glUniform3fv(location, 1, glm::value_ptr(values));
	}

	void GlShader::SetFloat4(const std::string& name, const glm::vec4& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		ASSERT(location != -1, "'" + name + "' does not correspond to an active uniform variable in this shader!", name);
		glUniform4fv(location, 1, glm::value_ptr(values));
	}

	void GlShader::SetMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		ASSERT(location != -1, "'" + name + "' does not correspond to an active uniform variable in this shader!", name);
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void GlShader::SetMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		ASSERT(location != -1, "'" + name + "' does not correspond to an active uniform variable in this shader!", name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	std::string GlShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
			{
				LOG_ERROR("Could not read from file '{0}'", filepath);
			}
		}
		else
		{
			LOG_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> GlShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void GlShader::Compile(std::unordered_map<GLenum, std::string> shaderSources)
	{
		m_RendererID = 0;

		GLuint program = glCreateProgram();
		ASSERT(shaderSources.size() <= 3, "We only support 3 shaders for now");
		LOG_TRACE("Compile Shader: {}", m_Name);

		int glShaderIDIndex = 0;
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;

			if (type == GL_VERTEX_SHADER)
			{
				LOG_TRACE("\tCompile GL_VERTEX_SHADER...");
			}
			if (type == GL_GEOMETRY_SHADER)
			{
				LOG_TRACE("\tCompile GL_GEOMETRY_SHADER...");
			}
			if (type == GL_FRAGMENT_SHADER)
			{
				LOG_TRACE("\tCompile GL_FRAGMENT_SHADER...");
			}

			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				LOG_ERROR("{0}", infoLog.data());
				ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			m_ShaderIDs.push_back(shader);
		}

		m_RendererID = program;
	}

	void GlShader::Link()
	{
		LOG_TRACE("Link Shader: {}", m_Name);
		glLinkProgram(m_RendererID);

		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(m_RendererID);

			for (auto id : m_ShaderIDs)
				glDeleteShader(id);

			LOG_ERROR("{0}", infoLog.data());
			ASSERT(false, "Shader link failure!");
			return;
		}

		for (auto id : m_ShaderIDs)
		{
			glDetachShader(m_RendererID, id);
			glDeleteShader(id);
		}

		m_ShaderIDs.clear();
	}

}