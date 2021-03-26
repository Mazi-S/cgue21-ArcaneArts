#pragma once

#include "Engine/Scene/ScriptableEntity.h"

#include <entt.hpp>

namespace Engine::Component::Core {

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) { }
		TransformComponent(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale)
			: Translation(translation), Rotation(rotation), Scale(scale) { }
	};

	struct ParentComponent
	{
		entt::entity Parent;

		ParentComponent(entt::entity parent) : Parent(parent) { }
		ParentComponent(const ParentComponent&) = default;
	};


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