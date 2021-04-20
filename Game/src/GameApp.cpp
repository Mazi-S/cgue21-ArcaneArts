#include "ExampleLayer.h"
#include "PhysicsTest.h"
#include "HudLayer.h"
#include "MenuLayer.h"
#include "GameApp.h"

int main() {

	Engine::WindowSpecification windowSpec = Engine::WindowSpecification();
	windowSpec.Title = "Arcane Arts";

	// TODO: load YAML

	Engine::Scope<GameApp> app = Engine::CreateScope<GameApp>(windowSpec);
	app->Run();
}

GameApp::GameApp(Engine::WindowSpecification windowSpec)
	: Application(windowSpec)
{
	PushOverlay(new MenuLayer());
	PushOverlay(new HudLayer());
	PushLayer(new ExampleLayer());
}

void GameApp::Victory()
{
	Get().OnEvent(GameEndEvent(true));
}

void GameApp::Defeat()
{
	Get().OnEvent(GameEndEvent(false));
}
