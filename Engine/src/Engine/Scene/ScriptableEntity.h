#pragma once

#include "Entity.h"

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

		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnEvent(Event e) {}

	protected:
		ScriptableEntity(Entity entity)
			: m_Entity(entity)
		{ }

	private:
		Entity m_Entity;
	};

}