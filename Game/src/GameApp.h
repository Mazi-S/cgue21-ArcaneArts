#pragma once
#include "Engine.h"

class GameApp : public Engine::Application
{
public:
	GameApp(Engine::ApplicationSpecification appSpec, Engine::WindowSpecification windowSpec);

	~GameApp() = default;

	static Engine::Layer s_GameLayer;

	static void Victory();
	static void Defeat();
};
