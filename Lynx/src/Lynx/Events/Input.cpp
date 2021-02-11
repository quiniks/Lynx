#include "Lynxpch.h"
#include "Lynx/Events/Input.h"
#include "Lynx/Core/App.h"
#include <GLFW/glfw3.h>

namespace Lynx {
	bool Input::IsKeyPressed(KeyCode key)
	{
		GLFWwindow* window = App::Get().GetWindow().GetGLFWWindow();
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		GLFWwindow* window = App::Get().GetWindow().GetGLFWWindow();
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}
	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePos();
		return y;
	}
	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePos();
		return x;
	}
	std::pair<float, float> Input::GetMousePos()
	{
		GLFWwindow* window = App::Get().GetWindow().GetGLFWWindow();
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}
}