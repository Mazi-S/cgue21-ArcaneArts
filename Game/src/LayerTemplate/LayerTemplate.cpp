#include "LayerTemplate.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

LayerTemplate::LayerTemplate()
	: Layer("HUD")
{
}

void LayerTemplate::OnAttach()
{
}

void LayerTemplate::OnDetach()
{
}

void LayerTemplate::OnUpdate(Engine::Timestep ts)
{
}

void LayerTemplate::OnEvent(Engine::Event& event)
{
}
