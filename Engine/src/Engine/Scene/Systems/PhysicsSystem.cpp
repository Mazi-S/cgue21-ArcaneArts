#include "egpch.h"
#include "PhysicsSystem.h"
#include "Engine/Scene/Components.h"

#include "PxPhysicsAPI.h"

namespace Engine::System::Physics {

	// source: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
	glm::vec3 ToEulerAngles(physx::PxQuat q) {
		glm::vec3 angles;

		// roll (x-axis rotation)
		double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
		double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
		angles.x = std::atan2(sinr_cosp, cosr_cosp);

		// pitch (y-axis rotation)
		double sinp = 2 * (q.w * q.y - q.z * q.x);
		if (std::abs(sinp) >= 1)
			angles.y = std::copysign(glm::half_pi<float>(), sinp); // use 90 degrees if out of range
		else
			angles.y = std::asin(sinp);

		// yaw (z-axis rotation)
		double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
		double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
		angles.z = std::atan2(siny_cosp, cosy_cosp);

		return angles;
	}

	void OnUpdate(entt::registry& registry)
	{
		auto view = registry.view<RegidDynamicComponent, TransformComponent>();
		for (const entt::entity e : view)
		{
			auto& [rdc, tc] = view.get<RegidDynamicComponent, TransformComponent>(e);
	
			physx::PxTransform t = rdc.Actor->getGlobalPose();
			tc.Translation = { t.p.x, t.p.y, t.p.z };
			tc.Rotation = ToEulerAngles(t.q);
		}
	}

}

