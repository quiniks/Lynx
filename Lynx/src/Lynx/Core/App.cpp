#include "Lynxpch.h"
#include "App.h"

namespace Lynx {
	#define BIND_EVENT_FN(x) std::bind(&App::x, this, std::placeholders::_1)

	App::App(const std::string& name)
	{
		m_Window = Window::Create(WindowProps());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		LX_CORE_INFO("App created");
	}

	App::~App()
	{
	}

	void App::Run()
	{
		while (true) {
			for(Layer* layer : m_LayerStack)
				layer->OnUpdate();
			m_Window->OnUpdate();
		}
	}

	void App::OnEvent(Event& e)
	{
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}
}