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

		const glm::mat4 ViewProjection() const
		{
			return Projection * glm::inverse(Transform);
		}


		const glm::vec3 Position() const { return { Transform[3][0], Transform[3][1], Transform[3][2] }; }
	};

}