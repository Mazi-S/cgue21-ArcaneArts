#include "Engine.h"

#include "ExampleLayer.h"
#include "PhysicsTest.h"
#include "HudLayer.h"

class GameApp : public Engine::Application
{
public:
	GameApp()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new HudLayer());
		//PushLayer(new PhysicsTestLayer());
	}

	~GameApp()
	{
	}
};

Engine::Application* Engine::CreateApplication() {
	return new GameApp();
}

int main() {
	Engine::Scope<GameApp> app = Engine::CreateScope<GameApp>();
	app->Run();
}