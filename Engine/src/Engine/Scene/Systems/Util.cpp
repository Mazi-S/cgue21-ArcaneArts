#include "egpch.h"
#include "Util.h"

#include <glm/glm.hpp>

namespace Engine::System::Util {

	glm::mat4 Transform(const TransformComponent& tc)
	{
		return glm::translate(glm::mat4(1.0f), tc.Translation)
			* glm::toMat4(glm::quat(tc.Rotation))
			* glm::scale(glm::mat4(1.0f), tc.Scale);
	}

	void RecalculateProjection(CameraComponent& cc)
	{
		cc.Projection = glm::perspective(cc.FOV, cc.AspectRatio, cc.Near, cc.Far);
	}

}

