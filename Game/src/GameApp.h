#pragma once
#include "Engine.h"

class GameApp : public Engine::Application
{
public:
	GameApp(Engine::WindowSpecification windowSpec);

	~GameApp() = default;

	static void Victory();
	static void Defeat();
};
