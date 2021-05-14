#include "egpch.h"
#include "Engine/Core/Input.h"

#include "Platform/Windows/WindowImpl.h"
#include "GLFW/glfw3.h"
#include "Engine/Core/Application.h"

namespace Engine {

	bool Input::IsKeyPressed(const KeyCode keycode) {
		Window* window = &Application::Get().GetWindow();
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(window->GetNativeWindow());
		auto state = glfwGetKey(glfwWindow, static_cast<int32_t>(keycode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(const MouseCode button) {
		Window* window = &Application::Get().GetWindow();
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(window->GetNativeWindow());
		auto state = glfwGetMouseButton(glfwWindow, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition() {
		Window* window = &Application::Get().GetWindow();
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(window->GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(glfwWindow, &xpos, &ypos);
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