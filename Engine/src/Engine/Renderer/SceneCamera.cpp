#include "egpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Engine {

	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float verticalFOV, float nearClip, float farClip)
	{
		m_FOV = verticalFOV;
		m_Near = nearClip;
		m_Far = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_Near, m_Far);
	}

}