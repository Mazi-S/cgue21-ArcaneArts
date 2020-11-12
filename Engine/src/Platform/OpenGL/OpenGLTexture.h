#pragma once

#include "Engine/Renderer/Texture.h"
#include "glm/glm.hpp"

namespace Engine::OpenGL {

	class Texture : public Engine::Texture
	{
	public:
		Texture(const char* filepath);
		virtual ~Texture();

		virtual void Bind(uint32_t slot) const override;
		virtual void Unbind() const override;

	private:
		GLuint LoadDDS(const char* filepath);

	private:
		GLuint m_TextureID;
	};

}
