#include "Lynxpch.h"
#include "Lynx/Core/Window.h"

#ifdef LX_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Lynx
{
	std::unique_ptr<IWindow> IWindow::Create(const WindowProps& props)
	{
		#ifdef LX_PLATFORM_WINDOWS
		return std::make_unique<WindowsWindow>(props);
		#else
		LX_CORE_ASSERT(false, "Unknown platform");
		return nullptr;
		#endif
	}
}