#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Material.h"
#include "Engine/Scene/ScriptableEntity.h"

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
			: Translation(translation) {}

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

		float TranslationSpeed = 7.0f;
		float RotationSpeed = 0.003f;
		float MouseX = 0, MouseY = 0;

		CharacterControllerComponent() = default;
		CharacterControllerComponent(const CharacterControllerComponent&) = default;
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
	// Parent /////////////////////////////////////////////////////////////////////////////////////
	struct HeroComponent
	{
		entt::entity LeftHand = entt::null;
		entt::entity RightHand = entt::null;

		HeroComponent() = default;
		HeroComponent(const HeroComponent&) = default;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// NativeScript ///////////////////////////////////////////////////////////////////////////////

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		template <typename T, typename... Args>
		void Bind(Args&&... args)
		{
			Instance = static_cast<ScriptableEntity*>(new T(std::forward<Args>(args)...));
			Instance->OnCreate();
		}

		void Unbind()
		{
			delete Instance;
			Instance = nullptr;
		}
	};

}