#pragma once

#include "Lynx/Events/Event.h"

namespace Lynx {

	template<typename T>
	class KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }
		static EventCategory GetStaticCategory() { return EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput; }
		virtual EventCategory GetCategoryFlags() const override { return T::GetStaticCategory(); }
		virtual EventType GetEventType() const override { return T::GetStaticType(); }
	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}

		int m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent<KeyPressedEvent>
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::KeyPressed; }
	private:
		int m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent<KeyReleasedEvent>
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::KeyReleased; }
	};

	class KeyTypedEvent : public KeyEvent<KeyTypedEvent>
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::KeyTyped; }
	};
}