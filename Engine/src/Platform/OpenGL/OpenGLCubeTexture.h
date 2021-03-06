#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

namespace Engine::OpenGL {

	class GlCubeTexture
	{
	public:
		GlCubeTexture(const std::string& name, std::vector<std::string> paths);
		virtual ~GlCubeTexture();

		virtual void Bind(uint32_t slot = 0) const;
		virtual const std::string& GetName() const { return m_Name; }
		virtual const std::vector<std::string>& GetPath() const { return m_Paths; }

	private:
		bool LoadCubemap();

	private:
		std::uint32_t m_TextureID;
		std::string m_Name;
		std::vector<std::string> m_Paths;

		uint32_t m_Width, m_Height;
		GLenum m_InternalFormat, m_DataFormat;
	};

}
