#include "Lynx/Core/Entry.h"

class SandBoxApp : public Lynx::App {
public:
	SandBoxApp() {
		LX_INFO("Hello");
	}

	~SandBoxApp() {

	}
};

Lynx::App* Lynx::Create() {
	return new SandBoxApp();
}