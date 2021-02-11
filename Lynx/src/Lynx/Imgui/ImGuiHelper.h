#pragma once

struct GLFWwindow;

namespace Lynx {
	class ImGuiHelper {
	public:
		ImGuiHelper();
		~ImGuiHelper();
		void Init(GLFWwindow* window);
		void Begin();
		void End();
	};
}
