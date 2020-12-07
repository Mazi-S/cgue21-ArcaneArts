#pragma once

#include "Entity.h"
#include "Scene.h"
#include "Engine/Events/Event.h"

namespace Engine {

	class ScriptableEntity
	{
		friend class Scene;

	public:
		virtual ~ScriptableEntity() = default;

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

		void Destroy()
		{
			m_Entity.Destroy();
		}

		template<typename T>
		void RemoveComponent()
		{
			m_Entity.RemoveComponent<T>();
		}

		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnEvent(Event& e) {}

	protected:
		ScriptableEntity(Entity entity)
			: m_Entity(entity), m_EntityHandle(entity.m_EntityHandle), m_RegistryHandle(entity.m_Scene->m_Registry), m_Scene(entity.m_Scene)
		{ }

	protected:
		entt::entity m_EntityHandle;
		entt::registry& m_RegistryHandle;
		Scene* m_Scene;

	private:
		Entity m_Entity;
	};

}