#pragma once

#ifdef LX_PLATFORM_WINDOWS

extern Lynx::App* Lynx::Create();

int main(int argc, char** argv) {
	Lynx::Log::Init();
	
	LX_PROFILE_BEGIN_SESSION("Startup", "Lynx-Profile-Startup.json");
	Lynx::App* app = Lynx::Create();
	LX_PROFILE_END_SESSION();

	LX_PROFILE_BEGIN_SESSION("Running", "Lynx-Profile-Running.json");
	app->Run();
	LX_PROFILE_END_SESSION();

	LX_PROFILE_BEGIN_SESSION("Shutdown", "Lynx-Profile-Shutdown.json");
	delete app;
	LX_PROFILE_END_SESSION();
}

#endif