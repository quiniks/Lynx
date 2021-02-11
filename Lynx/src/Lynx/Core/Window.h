#pragma once

#include "Lynx/Events/Event.h"
#include <functional>

struct GLFWwindow;

namespace Lynx {
	using EventCallbackFn = std::function<void(Event&)>;

	struct WindowProps {
		std::string Title = "LYNX";
		unsigned int Width = 1080;
		unsigned int Height = 720;
	};

	class Window {
	public:
		static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps());

		Window(const WindowProps& props);
		virtual ~Window();

		void OnUpdate();

		unsigned int GetWidth() const { return m_Data.Width; }
		unsigned int GetHeight() const { return m_Data.Height; }

		void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled);
		bool IsVSync() const;

		GLFWwindow* GetGLFWWindow() const { return m_Window; }

		void SetIcon(const std::string& filePath);
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		struct WindowData {
			std::string Title = "LYNX";
			unsigned int Width = 1080;
			unsigned int Height = 720;
			bool VSync = true;
			EventCallbackFn EventCallback = nullptr;
		} m_Data;
	};
}