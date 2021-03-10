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

	struct KinematicMovementComponent
	{
		// movement per second
		glm::vec3 Movement;
		glm::quat Rotation;

		KinematicMovementComponent(const KinematicMovementComponent&) = default;
		KinematicMovementComponent(glm::vec3 movement = { 0.0f, 0.0f, 0.0f }, glm::quat rotation = { 1.0f, 0.0f, 0.0f, 0.0f })
			: Movement(movement), Rotation(rotation) { }
	};

	struct RigidComponent
	{
		physx::PxRigidActor* Actor;

		RigidComponent(const RigidComponent&) = default;
		RigidComponent(physx::PxRigidActor* actor)
			: Actor(actor) { }
	};

	struct RigidDynamicComponent
	{
		physx::PxRigidDynamic* Actor;

		RigidDynamicComponent(const RigidDynamicComponent&) = default;
		RigidDynamicComponent(physx::PxRigidDynamic* actor)
			: Actor(actor) { }
	};

	struct ShapeComponent
	{
		physx::PxShape* Shape;

		ShapeComponent(const ShapeComponent&) = default;
		ShapeComponent(physx::PxShape* shape)
			: Shape(shape) { }
	};

	struct KinematicComponent
	{
		uint16_t temp = 0; // todo: fix

		KinematicComponent() = default;
		KinematicComponent(const KinematicComponent&) = default;
	};

	struct TriggerComponent
	{
		uint16_t temp = 0; // todo: fix

		TriggerComponent() = default;
		TriggerComponent(const TriggerComponent&) = default;
	};

	struct HitComponent
	{
		entt::entity Other;

		HitComponent(const HitComponent&) = default;
		HitComponent(entt::entity other)
			: Other(other) { }
	};

	struct MonsterComponent
	{
		uint16_t temp = 0; // todo: fix

		MonsterComponent() = default;
		MonsterComponent(const MonsterComponent&) = default;
	};

	struct MagicBallHitComponent
	{
		uint16_t temp = 0; // todo: fix

		MagicBallHitComponent() = default;
		MagicBallHitComponent(const MagicBallHitComponent&) = default;
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
	// Shadow /////////////////////////////////////////////////////////////////////////////////////

	struct ShadowComponent
	{
		uint16_t temp = 0; // todo: fix

		ShadowComponent() = default;
		ShadowComponent(const ShadowComponent&) = default;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// CharacterController ////////////////////////////////////////////////////////////////////////

	struct CharacterControllerComponent
	{
		bool Active = false;

		physx::PxController* Controller;

		float TranslationSpeed = 5.0f;
		float RotationSpeed = 0.002f;

		float StandingHeight; // total height of the character (including radius)
		float CrouchingHeight;
		float Radius;

		float MouseX = 0, MouseY = 0;
		float Jump = 0;
		bool Crouching = false;

		CharacterControllerComponent(float standingHeight, float crouchingHeight, float radius)
			: StandingHeight(standingHeight), CrouchingHeight(crouchingHeight), Radius(radius) { }
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