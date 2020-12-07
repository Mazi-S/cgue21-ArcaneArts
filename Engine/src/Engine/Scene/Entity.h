#pragma once

#include "entt.hpp"
#include "Scene.h"

namespace Engine {

	class Entity
	{
		friend class Scene;
		friend class ScriptableEntity;
		
	public:
		Entity() = default;
		Entity(entt::entity entity, Scene* scene);
		Entity(const Entity& other) = default;

		~Entity() = default;

		template <typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			ASSERT(!HasComponent<T>(), "Entity already has component!");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template <typename T, typename... Args>
		void AddNativeScript(Args&&... args)
		{
			ASSERT(!HasComponent<NativeScriptComponent>(), "Entity already has a native script!");
			NativeScriptComponent& nsc = m_Scene->m_Registry.emplace<NativeScriptComponent>(m_EntityHandle);
			nsc.Bind<T>(Entity(m_EntityHandle, m_Scene), std::forward<Args>(args)...);
		}

		template <typename T>
		T& GetComponent()
		{
			ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template <typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		void Destroy()
		{
			m_Scene->DestroyEntity(Entity(m_EntityHandle, m_Scene));
			m_Scene = nullptr;
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		operator entt::entity() const { return m_EntityHandle; }

		bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
		bool operator!=(const Entity& other) const { return !(*this == other); }

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};

}
