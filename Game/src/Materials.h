#pragma once

#include <Engine.h>

class ColorMaterial : public Engine::Material
{
public:
	ColorMaterial(const glm::vec4& color);

private:
	glm::vec4 m_Color;
};

