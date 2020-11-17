#pragma once

#include <Engine.h>

class ColorMaterial : public Engine::Material
{
public:
	ColorMaterial(const glm::vec4& color);

private:
	glm::vec4 m_Color;
};

class TextureMaterial : public Engine::Material
{
public:
	TextureMaterial(const std::string& filepath);
	virtual void Bind() override;

private:
	Engine::Ref<Engine::Texture> m_Texture;
};

