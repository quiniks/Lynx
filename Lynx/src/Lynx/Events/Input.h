#pragma once
#include "Lynx/Events/KeyCodes.h"
#include "Lynx/Events/MouseCodes.h"

namespace Lynx {
	class Input {
	public:
		static bool IsKeyPressed(KeyCode Key);

		static bool IsMouseButtonPressed(MouseCode button);
		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float, float> GetMousePos();
	};
}