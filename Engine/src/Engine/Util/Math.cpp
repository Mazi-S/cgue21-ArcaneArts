#include "egpch.h"
#include "Math.h"

#include <glm/gtc/constants.hpp>

namespace Engine::Util {

	// source: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
	glm::vec3 Math::ToEulerAngles(physx::PxQuat q)
	{
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

	// source: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
	physx::PxQuat Math::ToQuaternion(glm::vec3 euler)  // yaw (Z), pitch (Y), roll (X)
	{
		// Abbreviations for the various angular functions
		double cy = cos(euler.z * 0.5);
		double sy = sin(euler.z * 0.5);
		double cp = cos(euler.y * 0.5);
		double sp = sin(euler.y * 0.5);
		double cr = cos(euler.x * 0.5);
		double sr = sin(euler.x * 0.5);

		physx::PxQuat q;
		q.w = cr * cp * cy + sr * sp * sy;
		q.x = sr * cp * cy - cr * sp * sy;
		q.y = cr * sp * cy + sr * cp * sy;
		q.z = cr * cp * sy - sr * sp * cy;

		return q;
	}

}