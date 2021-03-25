#pragma once
#include "Lynx/Events/Event.h"
#include "Lynx/Events/AppEvent.h"
#include "Lynx/Core/Window.h"
#include "Lynx/Core/LayerStack.h"
#include "Lynx/Imgui/ImGuiHelper.h"
#include <functional>

namespace Lynx {
	class App {
	public:
		App(const std::string& name = "LYNX");
		~App();
		void Run();
		void OnEvent(Event& e);
		static App& Get() { return *s_Instance; }
		Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiHelper m_ImGuiHelper;
		bool m_Running = true;
		bool m_Minimized = false;
		float m_LastFrameTime = 0.0f;
		static App* s_Instance;
	protected:
		LayerStack m_LayerStack;
	};

	App* Create();
}