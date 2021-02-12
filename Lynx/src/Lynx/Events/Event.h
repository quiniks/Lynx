#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include "Lynx/Utility/BitMask.h"

namespace Lynx {
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFrameResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum class EventCategory
	{
		None = 0,							//00000
		EventCategoryApplication = BIT(0),	//00001
		EventCategoryInput = BIT(1),		//00010
		EventCategoryKeyboard = BIT(2),		//00100
		EventCategoryMouse = BIT(3),		//01000
		EventCategoryMouseButton = BIT(4)	//10000
	};
	ENABLE_BITMASK_OPERATORS(EventCategory);

	class Event
	{
	public:
		Event() = default;
		virtual ~Event() = default;

		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual EventCategory GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return std::to_string((char)GetEventType()); }

		bool IsInCategory(EventCategory category)
		{
			return bool(GetCategoryFlags() & category);
		}
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event) : m_Event(event) {}

		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};
	
	
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}