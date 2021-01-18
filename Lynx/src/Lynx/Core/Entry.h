#pragma once
#include "Lynx/Core/App.h"
#include "Lynx/Core/Log.h"

extern Lynx::App* Lynx::Create();

int main(int argc, char** argv) {
	Lynx::Log::Init();
	Lynx::App* app = Lynx::Create();
	app->Run();
	delete app;
}