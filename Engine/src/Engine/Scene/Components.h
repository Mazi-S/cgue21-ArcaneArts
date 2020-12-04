#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Material.h"
#include "Engine/Scene/ScriptableEntity.h"

namespace Engine {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// TagComponent ///////////////////////////////////////////////////////////////////////////////

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// TransformComponent /////////////////////////////////////////////////////////////////////////

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		operator glm::mat4() { return GetTransform(); }

	private:
		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// MeshComponent //////////////////////////////////////////////////////////////////////////////

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
	// MaterialComponent //////////////////////////////////////////////////////////////////////////

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
	// NativeScriptComponent //////////////////////////////////////////////////////////////////////

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

	///////////////////////////////////////////////////////////////////////////////////////////////
	// CharacterControllerComponent ///////////////////////////////////////////////////////////////

	struct CharacterControllerComponent
	{
		//CharacterController Controller;
		bool Active;

		CharacterControllerComponent() = default;
		CharacterControllerComponent(const CharacterControllerComponent&) = default;
	};
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	// CameraComponent ////////////////////////////////////////////////////////////////////////////

	struct CameraComponent
	{
		SceneCamera Camera;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

}