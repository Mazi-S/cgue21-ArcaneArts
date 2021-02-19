#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Material.h"
#include "Engine/Scene/ScriptableEntity.h"

#include "PxPhysicsAPI.h"

namespace Engine {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Tag ////////////////////////////////////////////////////////////////////////////////////////

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Transform //////////////////////////////////////////////////////////////////////////////////

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) { }
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Physics ////////////////////////////////////////////////////////////////////////////////////

	struct RegidDynamicComponent
	{
		physx::PxRigidActor* Actor;

		RegidDynamicComponent() = default;
		RegidDynamicComponent(const RegidDynamicComponent&) = default;
		RegidDynamicComponent(physx::PxRigidActor* actor)
			: Actor(actor) { }
	};

	struct RegidStaticComponent
	{
		physx::PxRigidActor* Actor;

		RegidStaticComponent() = default;
		RegidStaticComponent(const RegidStaticComponent&) = default;
		RegidStaticComponent(physx::PxRigidActor* actor)
			: Actor(actor) { }
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Mesh ///////////////////////////////////////////////////////////////////////////////////////

	struct MeshComponent
	{
		Ref<Engine::Mesh> Mesh;

		operator Ref<Engine::Mesh>() { return Mesh; }

		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(Ref<Engine::Mesh> mesh)
			: Mesh(mesh) { }
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Material ///////////////////////////////////////////////////////////////////////////////////

	struct MaterialComponent
	{
		Ref<Engine::Material> Material;

		operator Ref<Engine::Material>() { return Material; }

		MaterialComponent() = default;
		MaterialComponent(const MaterialComponent&) = default;
		MaterialComponent(Ref<Engine::Material> material)
			: Material(material) { }
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// CharacterController ////////////////////////////////////////////////////////////////////////

	struct CharacterControllerComponent
	{
		bool Active = false;

		physx::PxController* Controller;

		float TranslationSpeed = 7.0f;
		float RotationSpeed = 0.003f;
		float JumpHeight = 2;

		
		float Height; // total height of the character (including radius)
		float Radius;
		float CrouchHeight; // Height * CrouchHeight = crouching height

		float MouseX = 0, MouseY = 0;
		float Jump = 0;
		bool Crouching = false;

		CharacterControllerComponent(float height, float radius, float crouchHeight)
			: Height(height), Radius(radius), CrouchHeight(crouchHeight) { }
		CharacterControllerComponent(const CharacterControllerComponent&) = default;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// DirectionalLight ///////////////////////////////////////////////////////////////////////////

	struct DirectionalLightComponent
	{
		glm::vec3 Direction{ 0.0f, -1.0f, -0.5f };
		glm::vec3 Color{ 0.8f, 0.8f, 0.8f };

		DirectionalLightComponent() = default;
		DirectionalLightComponent(const glm::vec3& direction, const glm::vec3& color)
			: Direction(direction), Color(color) { }
		DirectionalLightComponent(const DirectionalLightComponent&) = default;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// PointLight /////////////////////////////////////////////////////////////////////////////////

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

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Camera /////////////////////////////////////////////////////////////////////////////////////

	struct CameraComponent
	{
		glm::mat4 Projection = glm::mat4(1.0f);

		float FOV = glm::radians(45.0f);
		float Near = 0.01f, Far = 1000.0f;

		float AspectRatio = 1.0f;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Parent /////////////////////////////////////////////////////////////////////////////////////
	
	struct ParentComponent
	{
		entt::entity Parent;

		ParentComponent(entt::entity parent) : Parent(parent) { }
		ParentComponent(const ParentComponent&) = default;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// NativeScript ///////////////////////////////////////////////////////////////////////////////

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		bool Active = false;

		template <typename T, typename... Args>
		void Bind(Args&&... args)
		{
			Instance = static_cast<ScriptableEntity*>(new T(std::forward<Args>(args)...));
			Instance->OnCreate();
			Active = true;
		}

		void Unbind()
		{
			Instance->OnDestroy();
			delete Instance;
			Instance = nullptr;
		}
	};

}