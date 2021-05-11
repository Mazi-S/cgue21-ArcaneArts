#pragma once

#include "entt.hpp"

namespace Engine {

	class Entity
	{
		friend class Scene;
		friend class ScriptableEntity;
		
	public:
		Entity() = default;
		Entity(entt::entity entity, entt::registry* registry);
		Entity(const Entity& other) = default;

		~Entity() = default;

		template <typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			ASSERT(!HasComponent<T>(), "Entity already has component!");
			return m_Registry->emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template <typename T, typename... Args>
		void AddNativeScript(Args&&... args)
		{
			ASSERT(!HasComponent<Component::Core::NativeScriptComponent>(), "Entity already has a native script!");
			Component::Core::NativeScriptComponent& nsc = m_Registry->emplace<Component::Core::NativeScriptComponent>(m_EntityHandle);
			nsc.Bind<T>(Entity(m_EntityHandle, m_Registry), std::forward<Args>(args)...);
		}

		template <typename T, typename... Args>
		T& EmplaceOrReplace(Args&&... args)
		{
			return m_Registry->emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template <typename T>
		T& GetComponent()
		{
			ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Registry->get<T>(m_EntityHandle);
		}

		template <typename T>
		bool HasComponent() const
		{
			return m_Registry->has<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Registry->remove<T>(m_EntityHandle);
		}

		void Destroy()
		{
			m_Registry->destroy(m_EntityHandle);
			m_Registry = nullptr;
		}

		template<typename T>
		void Update()
		{
			m_Registry->patch<T>(m_EntityHandle, [](auto &comp) { });
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		operator entt::entity() const { return m_EntityHandle; }

		bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle; }
		bool operator!=(const Entity& other) const { return !(*this == other); }

		std::string ToString() const;

	private:
		entt::entity m_EntityHandle{ entt::null };
		entt::registry* m_Registry = nullptr;
	};

}
