#include "ExampleLayer.h"

#include <iostream>
#include <Engine/Events/KeyEvent.h>

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

	// Render here
}

static bool printKey(Engine::KeyPressedEvent e)
{
	std::stringstream ss;
	ss << "KeyPressedEvent: " << e.GetKeyCode();
	LOG_INFO(ss.str());
	return false;
}

void ExampleLayer::OnEvent(Engine::Event& event)
{
	// just for testing
	Engine::EventHandler eventHandler(event);
	eventHandler.Handle<Engine::KeyPressedEvent>(printKey);
}


