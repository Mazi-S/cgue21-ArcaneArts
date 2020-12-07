#pragma once

#include <glm/glm.hpp>

namespace Engine {

	struct Camera
	{
		glm::mat4 Projection;
		glm::mat4 Transform;

		Camera(const glm::mat4& projection = glm::mat4(1.0f), const glm::mat4& transform = glm::mat4(1.0f))
			: Projection(projection), Transform(transform)
		{ }
	};

}