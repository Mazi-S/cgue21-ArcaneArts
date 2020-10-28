#pragma once
#include "Camera.h"

namespace Engine {

	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetPerspective(float verticalFOV, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

		float GetVerticalFOV() const { return m_FOV; }
		void SetVerticalFOV(float verticalFov) { m_FOV = verticalFov; RecalculateProjection(); }
		float GetNearClip() const { return m_Near; }
		void SetNearClip(float nearClip) { m_Near = nearClip; RecalculateProjection(); }
		float GetFarClip() const { return m_Far; }
		void SetFarClip(float farClip) { m_Far = farClip; RecalculateProjection(); }

	private:
		void RecalculateProjection();

	private:
		float m_FOV = glm::radians(45.0f);
		float m_Near = 0.01f, m_Far = 1000.0f;

		float m_AspectRatio = 1.0f;
	};

}
