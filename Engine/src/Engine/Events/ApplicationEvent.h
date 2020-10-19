#pragma once
#include "Event.h"

namespace Engine {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// WindowResizeEvent //////////////////////////////////////////////////////////////////////////

	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) {}

		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Height; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(static_cast<uint32_t>(EventCategory::Application))
	private:
		unsigned int m_Width, m_Height;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// WindowCloseEvent ///////////////////////////////////////////////////////////////////////////

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(static_cast<uint32_t>(EventCategory::Application))
	};

}