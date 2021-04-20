#pragma once
#include "Engine.h"

class GameApp : public Engine::Application
{
public:
	GameApp();

	~GameApp() = default;

	static void Victory();
	static void Defeat();
};
