#include "ExampleLayer.h"
#include "PhysicsTest.h"
#include "HudLayer.h"
#include "MenuLayer.h"
#include "GameApp.h"

int main() {

	auto config = Engine::Util::Config("config.yaml");

	Engine::ApplicationSpecification appSpec = Engine::ApplicationSpecification();
	appSpec.Brightness = config.GetBrightness();
	appSpec.Brightness2D = config.GetBrightness2D();

	Engine::WindowSpecification windowSpec = Engine::WindowSpecification();
	windowSpec.Title = "Arcane Arts";
	windowSpec.Width = config.GetWidth();
	windowSpec.Height = config.GetHeight();
	windowSpec.RefreshRate = config.GetRefreshRate();
	windowSpec.fullscreen = config.GetFullscreen();

	Engine::Scope<GameApp> app = Engine::CreateScope<GameApp>(appSpec, windowSpec);
	app->Run();
}

GameApp::GameApp(Engine::ApplicationSpecification appSpec, Engine::WindowSpecification windowSpec)
	: Application(appSpec, windowSpec)
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
