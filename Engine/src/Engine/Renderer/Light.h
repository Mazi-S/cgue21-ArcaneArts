#pragma once

#include <glm/glm.hpp>

namespace Engine {

	struct DirectionalLight
	{
		glm::vec3 Direction;
		glm::vec3 Color;

		DirectionalLight(const glm::vec3& direction = glm::vec3(0.0f, -1.0f, 0.0f) , const glm::vec3& color = glm::vec3(0.0f, 0.0f, 0.0f))
			: Direction(direction), Color(color)
		{ }
	};

	struct PointLight
	{
		glm::vec3 Position;
		glm::vec3 Color;
		float Constant;
		float Linear;
		float Quadratic;

		PointLight(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& color = glm::vec3(0.0f, 0.0f, 0.0f), float constant = 1.0f, float linear = 0.4f, float quadratic = 0.1f)
			: Position(position), Color(color), Constant(constant), Linear(linear), Quadratic(quadratic)
		{ }
	};

}