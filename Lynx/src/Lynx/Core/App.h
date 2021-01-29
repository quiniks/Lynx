#pragma once
#include "Lynx/Events/Event.h"
#include "Lynx/Events/AppEvent.h"
#include "Lynx/Core/Window.h"
#include "Lynx/Core/LayerStack.h"
#include <functional>

namespace Lynx {
	class App {
	public:
		App(const std::string& name = "LYNX");
		~App();
		void Run();
		void OnEvent(Event& e);
	private:
		bool OnWindowClosed(WindowCloseEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		float m_LastFrameTime = 0.0f;
	protected:
		LayerStack m_LayerStack;
	};

	App* Create();
}