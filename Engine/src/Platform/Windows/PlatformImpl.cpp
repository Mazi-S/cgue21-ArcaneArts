#include "egpch.h"
#include "Platform/Platform.h"
#include "GLFW/glfw3.h"

namespace Engine {

	float Platform::GetTime() {
		return (float)glfwGetTime();
	}

}