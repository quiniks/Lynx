#pragma once

#include <GLFW/glfw3.h>
#include "Lynx/Core/Window.h"

namespace Lynx {
	class WindowsWindow : public IWindow {
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }

		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return m_Window; }

		void SetIcon(const std::string& filePath) override;
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		//RenderContext* m_Context;

		struct WindowData {
			std::string Title = "";
			unsigned int Width = 1080;
			unsigned int Height = 720;
			bool VSync = true;
			EventCallbackFn EventCallback = nullptr;
		} m_Data;
	};
}