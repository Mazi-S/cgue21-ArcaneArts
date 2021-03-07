#pragma once

#include <glad/glad.h>

namespace Engine::OpenGL {

	class GlTexture2D
	{
	public:
		GlTexture2D(const std::string& name, const std::string& filepath);
		virtual ~GlTexture2D();

		virtual void Bind(uint32_t slot = 0) const;
		virtual const std::string& GetName() const { return m_Name; }
		virtual const std::string& GetPath() const { return m_Path; }

	private:
		bool Load();

	private:
		std::uint32_t m_TextureID;
		std::string m_Name;
		std::string m_Path;

		uint32_t m_Width, m_Height;
		GLenum m_InternalFormat, m_DataFormat;
	};

}
