#pragma once
#include "Engine/Events/Event.h"
#include "Engine/Core/KeyCodes.h"

namespace Engine {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// KeyEvent ///////////////////////////////////////////////////////////////////////////////////

	class KeyEvent : public Event {
	public:
		inline KeyCode GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategory::Keyboard | EventCategory::Input)
	protected:
		KeyEvent(const KeyCode keycode) : m_KeyCode(keycode) {}

		KeyCode m_KeyCode;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// KeyPressedEvent ////////////////////////////////////////////////////////////////////////////

	class KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(const KeyCode keycode, bool repeated) : KeyEvent(keycode), m_Repeated(repeated) {}

		inline bool IsRepeated() const { return m_Repeated; }

		std::string ToString() const override {
			std::stringstream ss;

			ss << "KeyPressedEvent: " << m_KeyCode;
			if (m_Repeated) { ss << " (Repeated)"; }
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		bool m_Repeated;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// KeyReleasedEvent ///////////////////////////////////////////////////////////////////////////

	class KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(KeyCode keycode) : KeyEvent(keycode) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// KeyTypedEvent //////////////////////////////////////////////////////////////////////////////

	class KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(KeyCode keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}