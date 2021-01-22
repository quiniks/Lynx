#include "Lynxpch.h"
#include "App.h"

namespace Lynx {
	#define BIND_EVENT_FN(x) std::bind(&App::x, this, std::placeholders::_1)

	App::App(const std::string& name)
	{
		m_Window = IWindow::Create(WindowProps(name));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	App::~App()
	{
	}

	void App::Run()
	{
		while (true) {
			m_Window->OnUpdate();
		}
	}

	void App::OnEvent(Event& e)
	{
		LX_CORE_INFO(e);
	}
}