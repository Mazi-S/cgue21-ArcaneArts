#include "ExampleLayer.h"

#include <iostream>

ExampleLayer::ExampleLayer()
	: Layer("Example")
{
}

void ExampleLayer::OnAttach()
{
	// Init here
}

void ExampleLayer::OnDetach()
{
	// Shutdown here
}

void ExampleLayer::OnUpdate(Engine::Timestep ts)
{
	// Update here
	static uint64_t i = 0;
	if (i > 15000)
	{
		Engine::WindowCloseEvent closeEvent;
		Engine::Application::Get().OnEvent(closeEvent);
	}

	// Render here

	// just for testing
	std::cout << "Test: " << i++ << std::endl;
}

void ExampleLayer::OnEvent(Engine::Event& event)
{

}

