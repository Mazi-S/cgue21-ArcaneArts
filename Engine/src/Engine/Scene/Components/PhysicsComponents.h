#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <PxPhysicsAPI.h>

namespace Engine::Component::Physics {

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

	struct CharacterControllerComponent
	{
		bool Active = false;

		physx::PxController* Controller = nullptr;

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

}