#pragma once
#include "Lynx/Core/Window.h"
#include "Lynx/Events/Event.h"

namespace Lynx {
	class App {
	public:
		App(const std::string& name = "LYNX");
		~App();
		void Run();
		void OnEvent(Event& e);
	private:
		std::unique_ptr<IWindow> m_Window;
	};

	App* Create();
}