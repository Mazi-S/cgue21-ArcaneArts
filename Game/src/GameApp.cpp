#include "ExampleLayer.h"
#include "PhysicsTest.h"
#include "HudLayer.h"
#include "MenuLayer.h"
#include "GameApp.h"

int main() {
	Engine::Scope<GameApp> app = Engine::CreateScope<GameApp>();
	app->Run();
}

GameApp::GameApp()
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
