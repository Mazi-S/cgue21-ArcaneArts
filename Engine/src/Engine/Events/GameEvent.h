#pragma once
#include "Engine/Events/Event.h"

enum class GameEventType;

namespace Engine {

	class GameEvent : public Event {
	public:
		virtual GameEventType GetGameEventType() const = 0;

		EVENT_CLASS_TYPE(Game)
		EVENT_CLASS_CATEGORY(static_cast<uint32_t>(EventCategory::Game))
	};
}