#pragma once
#include "egpch.h"
#include "Engine/Core/Base.h"

namespace Engine {

	// Blocking Event System!

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
		Collision
	};

	enum class EventCategory {
		None = 0,
		Application   = BIT(0),
		Input         = BIT(1),
		Keyboard      = BIT(2),
		Mouse         = BIT(3),
		MouseButton   = BIT(4),
		Physics       = BIT(5)
	};

	uint32_t operator|(EventCategory lhs, EventCategory rhs);
	uint32_t operator|(uint32_t lhs, EventCategory rhs);
	uint32_t operator|(EventCategory lhs, uint32_t rhs);

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual uint32_t GetCategoryFlags() const override { return category; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Event //////////////////////////////////////////////////////////////////////////////////////

	class Event {
		friend class EventHandler;

	public:
		virtual ~Event() = default;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual uint32_t GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		bool Handled = false;

		inline bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & static_cast<uint32_t>(category);
		}

	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// EventHandler ///////////////////////////////////////////////////////////////////////////////

	class EventHandler {
	public:
		EventHandler(Event& event) : m_Event(event) {}

		template<typename T, typename F>
		bool Handle(F& func)
		{
			if (m_Event.GetEventType() == T::GetStaticType()) {
				m_Event.Handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}

}