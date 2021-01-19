#include "Lynxpch.h"
#include "App.h"

namespace Lynx {
	#define BIND_EVENT_FN(x) = std::bind(&Application::x, this, std::placeholders::_1)

	App::App(const std::string& name)
	{
		m_Window = IWindow::Create(WindowProps(name));
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

	void App::Close()
	{
	}
}