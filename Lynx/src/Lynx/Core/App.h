#pragma once

namespace Lynx {
	class App {
	public:
		App();
		~App();

		void Run();
		void Close();
	};
	App* Create();
}