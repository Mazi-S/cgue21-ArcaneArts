#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/MouseCodes.h"

namespace Engine {

	class Input {
	public:
		static bool IsKeyPressed(const KeyCode keycode);

		static bool IsMouseButtonPressed(const MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}