#include "egpch.h"
#include "Engine/Core/Input.h"

#include "Platform/Windows/WindowImpl.h"
#include "GLFW/glfw3.h"
#include "Engine/Core/Application.h"

namespace Engine {

	bool Input::IsKeyPressed(const KeyCode keycode) {
		auto* window = static_cast<WindowImpl*>(&Application::Get().GetWindow())->GetNativeWindow();
		auto state = glfwGetKey(window, static_cast<int32_t>(keycode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(const MouseCode button) {
		auto* window = static_cast<WindowImpl*>(&Application::Get().GetWindow())->GetNativeWindow();
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition() {
		auto* window = static_cast<WindowImpl*>(&Application::Get().GetWindow())->GetNativeWindow();
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

	float Input::GetMouseX() {
		auto [xpos, ypos] = Input::GetMousePosition();
		return xpos;
	}

	float Input::GetMouseY() {
		auto [xpos, ypos] = Input::GetMousePosition();
		return ypos;
	}

}