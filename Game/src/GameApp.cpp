#include "Engine.h"

#include "ExampleLayer.h"
#include "PhysicsTest.h"
#include "HudLayer.h"
#include "MenuLayer.h"

class GameApp : public Engine::Application
{
public:
	GameApp()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new HudLayer());
		PushOverlay(new MenuLayer());
		//PushLayer(new PhysicsTestLayer());
	}

	~GameApp()
	{
	}

};

int main() {
	Engine::Scope<GameApp> app = Engine::CreateScope<GameApp>();
	app->Run();
}
