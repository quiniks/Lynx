#include <Lynx.h>

class SandBoxApp : public Lynx::App {
public:
	SandBoxApp() : Lynx::App() {
		LX_INFO("Hello");
	}

	~SandBoxApp() {

	}
};

Lynx::App* Lynx::Create() {
	return new SandBoxApp();
}