#include "SkyboxLayer.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image\stb_image.h>

unsigned int LoadCubeMap(std::vector<std::string> faces);

SkyboxLayer::SkyboxLayer()
	: Layer("Skybox")
{
}

void SkyboxLayer::OnAttach()
{
}

void SkyboxLayer::OnDetach()
{
}

void SkyboxLayer::OnUpdate(Engine::Timestep ts)
{
}

void SkyboxLayer::OnEvent(Engine::Event& event)
{
}