#pragma once

#include "Lynx/Events/Event.h"

namespace Lynx {
	template<typename T>
	class AppEvent : public Event {
	public:
		static EventCategory GetStaticCategory() { return EventCategory::EventCategoryApplication; }
		virtual EventCategory GetCategoryFlags() const override { return T::GetStaticCategory(); }
		virtual EventType GetEventType() const override { return T::GetStaticType(); }
	};

	class WindowResizeEvent : public AppEvent<WindowResizeEvent>
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::WindowResize; }
	private:
		unsigned int m_Width, m_Height;
	};

	class WindowFrameResizeEvent : public AppEvent<WindowFrameResizeEvent>
	{
	public:
		WindowFrameResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowFrameResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::WindowFrameResize; }
	private:
		unsigned int m_Width, m_Height;
	};

	class WindowCloseEvent : public AppEvent<WindowCloseEvent>
	{
	public:
		WindowCloseEvent() = default;

		static EventType GetStaticType() { return EventType::WindowClose; }
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;

		static EventType GetStaticType() { return EventType::AppTick; }
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		static EventType GetStaticType() { return EventType::AppUpdate; }
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		static EventType GetStaticType() { return EventType::AppRender; }
	};
}