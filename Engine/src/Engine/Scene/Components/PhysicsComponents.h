#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <PxPhysicsAPI.h>

namespace Engine::Component::Physics {

	struct StaticColliderComponent
	{
		physx::PxRigidActor* Actor = nullptr;

		StaticColliderComponent() = default;
	};

	struct CharacterControllerComponent
	{
		bool Active = false;

		physx::PxController* Controller = nullptr;

		float TranslationSpeed;
		float RotationSpeed;

		float StandingHeight; // total height of the character (including radius)
		float CrouchingHeight;
		float Radius;

		float MouseX = 0, MouseY = 0;
		float Jump = 0;
		bool Crouching = false;

		CharacterControllerComponent(float standingHeight = 2.0f, float crouchingHeight = 1.2f, float radius = 0.3f, float translationSpeed = 5.0f, float rotationSpeed = 0.002f)
			: StandingHeight(standingHeight), CrouchingHeight(crouchingHeight), Radius(radius), TranslationSpeed(translationSpeed), RotationSpeed(rotationSpeed) { }
		CharacterControllerComponent(const CharacterControllerComponent&) = default;
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

	struct KinematicMovementComponent
	{
		// movement per second
		glm::vec3 Movement;
		glm::quat Rotation;

		KinematicMovementComponent(const KinematicMovementComponent&) = default;
		KinematicMovementComponent(glm::vec3 movement = { 0.0f, 0.0f, 0.0f }, glm::quat rotation = { 1.0f, 0.0f, 0.0f, 0.0f })
			: Movement(movement), Rotation(rotation) { }
	};

}