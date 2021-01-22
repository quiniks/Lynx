#include "Lynxpch.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Lynx/Events/AppEvent.h"
#include "Lynx/Events/MouseEvent.h"

namespace Lynx {
	static unsigned int s_GLFWWindowCount = 0;

	void GLFWErrorCallback(int error, const char* desc) {
		LX_CORE_ERROR("GLFW ERROR ({0}): {1}", error, desc);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {
		Init(props);
	}

	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}

	void WindowsWindow::SetIcon(const std::string& filePath)
	{
		//int width, height, nrChannels;
		//unsigned char* img = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 4);
		//GLFWimage icon = { width, height, img };
		//glfwSetWindowIcon(m_Window, 1, &icon);
		//stbi_image_free(img);
	}

	void WindowsWindow::Init(const WindowProps& props) {
		//LX_PROFILE_FUNCTION();

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		LX_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (s_GLFWWindowCount == 0) {
			//LX_PROFILE_SCOPE("glfwInit");
			int success = glfwInit();
			LX_ASSERT(success, "Could not initialise GLFW");
			glfwSetErrorCallback(GLFWErrorCallback);
		}
		
		{
			//LX_PROFILE_SCOPE("glfwCreateWindow");
			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}

		glfwMakeContextCurrent(m_Window);//test
		//Context = new OpenGLContext(m_Window);
		//m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action) {
				case GLFW_PRESS:
				{
					//KeyPressedEvent event(key, 0);
					//data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					//KeyReleasedEvent event(key);
					//data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					//KeyPressedEvent event(key, 1);
					//data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			//KeyTypedEvent event(keycode);
			//data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS: {
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
	}

	void WindowsWindow::Shutdown() {
		//LX_PROFILE_FUNCTION();
		glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;
		if (s_GLFWWindowCount == 0) {
			glfwTerminate();
		}
	}

	void WindowsWindow::OnUpdate() {
		//LX_PROFILE_FUNCTION();
		glfwPollEvents();
		glfwSwapBuffers(m_Window);//test
		//m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled) {
		//LX_PROFILE_FUNCTION();
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const {
		return m_Data.VSync;
	}
}