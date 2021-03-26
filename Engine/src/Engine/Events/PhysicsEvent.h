#pragma once
#include "Event.h"
#include "Engine/Scene/Entity.h"
#include <utility>

namespace Engine {

	class CollisionEvent : public Event
	{
	public:
		CollisionEvent(const Entity& first, const Entity& second)
			: m_First(first), m_Second(second) { }

		inline Entity GetFirst() { return m_First; }
		inline Entity GetSecond() { return m_Second; }
		inline std::pair<Entity, Entity> Get() { return { m_First, m_Second }; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "CollisionEvent: " << m_First.ToString() << ", " << m_Second.ToString();
			return ss.str();
        }

		EVENT_CLASS_TYPE(Collision)
		EVENT_CLASS_CATEGORY(static_cast<uint32_t>(EventCategory::Physics))

	private:
		Entity m_First;
		Entity m_Second;
	};

}