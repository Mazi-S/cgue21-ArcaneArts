#pragma once
#include <string>
#include "Engine/Renderer/ParticleSystem.h"

namespace Engine::Component::Renderer {

	struct MeshComponent
	{
		std::string Mesh;

		operator std::string() { return Mesh; }

		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(std::string mesh)
			: Mesh(mesh)
		{ }
	};

	struct MaterialComponent
	{
		std::vector<std::string> Materials;

		MaterialComponent() = default;
		MaterialComponent(const MaterialComponent&) = default;
		MaterialComponent(std::string material)
			: Materials({ material }) { }
		MaterialComponent(std::vector<std::string> materials)
			: Materials(materials) { }
	};
	
	struct ShadowComponent
	{
		uint16_t temp = 0; // todo: fix

		ShadowComponent() = default;
		ShadowComponent(const ShadowComponent&) = default;
	};

	struct DirectionalLightComponent
	{
		glm::vec3 Direction{ 0.0f, -1.0f, -0.5f };
		glm::vec3 Color{ 0.8f, 0.8f, 0.8f };

		DirectionalLightComponent() = default;
		DirectionalLightComponent(const glm::vec3& direction, const glm::vec3& color)
			: Direction(direction), Color(color) { }
		DirectionalLightComponent(const DirectionalLightComponent&) = default;
	};

	struct PointLightComponent
	{
		glm::vec3 Color{ 0.8f, 0.8f, 0.8f };
		float Constant = 1.0f;
		float Linear = 0.4f;
		float Quadratic = 0.1f;

		PointLightComponent() = default;
		PointLightComponent(const glm::vec3& color, float constant, float linear, float quadratic)
			: Color(color), Constant(constant), Linear(linear), Quadratic(quadratic) { }
		PointLightComponent(const PointLightComponent&) = default;
	};

	struct CameraComponent
	{
		glm::mat4 Projection = glm::mat4(1.0f);
		float AspectRatio = 1.0f;

		float FOV = glm::radians(45.0f);
		float Near = 0.01f, Far = 1000.0f;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(float fov, float nearPlane, float farPlane)
			: FOV(fov), Near(nearPlane), Far(farPlane) { }
	};

	// Effects

	struct ParticleSystemComponent
	{
		ParticleSystem* ParticleSystem = nullptr;

		ParticleSystemType Type = ParticleSystemType::Fire;
		float EmitPower = 0.001f;
		float Cooling = 0.05f;

		float ParticleSize = 0.01f;
		glm::vec4 ColorStart = { 1, 1, 0, 1 };
		glm::vec4 ColorEnd = { 1, 0, 0, 0 };

		ParticleSystemComponent() = default;
		ParticleSystemComponent(const ParticleSystemComponent&) = default;
		ParticleSystemComponent(ParticleSystemType type, float emitPower, float cooling, float particleSize, glm::vec4 colorStart, glm::vec4 colorEnd)
			: Type(type), EmitPower(emitPower), Cooling(cooling), ParticleSize(particleSize), ColorStart(colorStart), ColorEnd(colorEnd) { }
	};

}