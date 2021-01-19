#pragma once

#ifdef LX_PLATFORM_WINDOWS

extern Lynx::App* Lynx::Create();

int main(int argc, char** argv) {
	Lynx::Log::Init();
	Lynx::App* app = Lynx::Create();
	app->Run();
	delete app;
}

#endif