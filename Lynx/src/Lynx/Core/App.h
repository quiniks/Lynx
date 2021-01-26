#pragma once
#include "Lynx/Events/Event.h"
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
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
	};

	App* Create();
}