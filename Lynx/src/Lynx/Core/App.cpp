#include "Lynxpch.h"
#include "App.h"
#include "Lynx/Core/TimeStep.h"
#include <GLFW/glfw3.h>

namespace Lynx {
	App* App::s_Instance = nullptr;

	App::App(const std::string& name)
	{
		LX_CORE_ASSERT(!s_Instance, "Applcation already exists");
		LX_CORE_INFO("App created");
		s_Instance = this;
		m_Window = Window::Create(WindowProps());
		m_Window->SetEventCallback(BIND_EVENT_FN(App::OnEvent));
		m_ImGuiHelper.Init(m_Window->GetGLFWWindow());
	}

	App::~App()
	{
	}

	void App::Run()
	{
		while (m_Running) {
			float time = (float)glfwGetTime();
			TimeStep timeStep = time - m_LastFrameTime;
			m_LastFrameTime = time;
			for(auto& layer : m_LayerStack)
				layer->OnUpdate(timeStep);
			m_ImGuiHelper.Begin();
			for (auto& layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiHelper.End();
			m_Window->OnUpdate();
		}
	}

	void App::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(App::OnWindowClosed));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	bool App::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}