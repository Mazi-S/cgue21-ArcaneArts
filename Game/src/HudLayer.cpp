#include "HudLayer.h"

#include <Engine/Events/KeyEvent.h>
#include <Engine/Renderer/Texture.h>
#include <Engine/Util/ObjectLoader.h>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

HudLayer::HudLayer()
	: Layer("HUD")
{
}

void HudLayer::OnAttach()
{
}

void HudLayer::OnDetach()
{
}

void HudLayer::OnUpdate(Engine::Timestep ts)
{
}

void HudLayer::OnEvent(Engine::Event& event)
{
}
