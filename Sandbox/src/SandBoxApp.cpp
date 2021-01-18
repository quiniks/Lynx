#include "Lynx/Core/App.h"
#include "Lynx/Core/Entry.h"

class SandBoxApp : public Lynx::App {
public:
	SandBoxApp() {

	}

	~SandBoxApp() {

	}
};

Lynx::App* Lynx::Create() {
	return new SandBoxApp();
}