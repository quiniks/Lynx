#include "SandBoxLayer.h"
#include <Lynx.h>
#include <Lynx/Core/Entry.h>

class SandBoxApp : public Lynx::App {
public:
	SandBoxApp() : Lynx::App() {
		m_LayerStack.PushLayer(std::make_shared<SandBoxLayer>());
	}

	~SandBoxApp() {

	}
};

Lynx::App* Lynx::Create() {
	return new SandBoxApp();
}