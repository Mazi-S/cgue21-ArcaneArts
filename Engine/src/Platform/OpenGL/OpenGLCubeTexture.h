#pragma once

#include "Engine/Renderer/Texture.h"

#include <glm/glm.hpp>
#include <glad/glad.h>

namespace Engine::OpenGL {

	class CubeTexture : public Engine::Texture
	{
	public:
		CubeTexture(std::vector<std::string> faces);
		virtual ~CubeTexture();

		virtual void Bind(uint32_t slot) const override;
		virtual const std::string& GetName() const override { return m_Name; }
		virtual const std::string& GetPath() const override { return m_Path; }

	private:
		bool LoadCubemap(const std::vector<std::string> faces);

	private:
		std::uint32_t m_TextureID;
		std::string m_Name;
		std::string m_Path;

		uint32_t m_Width, m_Height;
		GLenum m_InternalFormat, m_DataFormat;
	};

}
