#pragma once
#include <iostream>
#include "Lynx/Utility/BitMask.h"


namespace Lynx {
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
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

	//Used instead of add these three functions each time for each event type
	#define EVENT_CLASS_TYPE(type)	static EventType GetStaticType() { return EventType::type; }\
									virtual EventType GetEventType() const override { return GetStaticType(); }\
									virtual const char* GetName() const override { return #type; }

	#define EVENT_CLASS_CATEGORY(category)	static EventCategory GetStaticCategoryFlags() { return category; }\
											virtual EventCategory GetCategoryFlags() const override { return GetStaticCategoryFlags(); }

	class Event
	{
	public:
		virtual ~Event() = default;

		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual EventCategory GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

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