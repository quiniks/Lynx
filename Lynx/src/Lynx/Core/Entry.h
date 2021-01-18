#pragma once

extern Lynx::App* Lynx::Create();

int main(int argc, char** argv) {
	Lynx::App* app = Lynx::Create();
	app->Run();
	delete app;
}