#pragma once

#include "Lynx/Events/Event.h"

namespace Lynx {
	template<typename T>
	class MouseEvent : public Event {
	public:
		static EventCategory GetStaticCategory() { return EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput; }
		virtual EventCategory GetCategoryFlags() const override { return T::GetStaticCategory(); }
		virtual EventType GetEventType() const override { return T::GetStaticType(); }
	};

	template<typename T>
	class MouseButtonEvent : public Event {
	public:
		int GetMouseButton() const { return m_Button; }
	public:
		static EventCategory GetStaticCategory() { return EventCategory::EventCategoryMouseButton | EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput; }
		virtual EventCategory GetCategoryFlags() const override { return T::GetStaticCategory(); }
		virtual EventType GetEventType() const override { return T::GetStaticType(); }
	protected:
		MouseButtonEvent(int button)
			: m_Button(button) {}

		int m_Button;
	};

	class MouseMovedEvent : public MouseEvent<MouseMovedEvent>
	{
	public:
		MouseMovedEvent(float x, float y)
			: m_MouseX(x), m_MouseY(y) {}

		float GetX() const { return m_MouseX; }
		float GetY() const { return m_MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::MouseMoved; }
	private:
		float m_MouseX, m_MouseY;
	};

	class MouseScrolledEvent : public MouseEvent<MouseScrolledEvent>
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {}

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::MouseScrolled; }
	private:
		float m_XOffset, m_YOffset;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent<MouseButtonPressedEvent>
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::MouseButtonPressed; }
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent<MouseButtonReleasedEvent>
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::MouseButtonReleased; }
	};

}