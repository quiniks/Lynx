#pragma once

#include <codeanalysis\warnings.h>
#pragma warning( push )
#pragma warning ( disable : ALL_CODE_ANALYSIS_WARNINGS )
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#pragma warning( pop )


namespace Lynx {
	class Log {
	public:
		static void Init();
		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger>& GetAppLogger() { return s_AppLogger; }
	private:
		Log() {}
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_AppLogger;
	};
}

//Core log macros
#define LX_CORE_TRACE(...)		Lynx::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LX_CORE_INFO(...)		Lynx::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LX_CORE_WARN(...)		Lynx::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LX_CORE_ERROR(...)		Lynx::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LX_CORE_CRITICAL(...)	Lynx::Log::GetCoreLogger()->critical(__VA_ARGS__)
//App log macros
#define LX_TRACE(...)			Lynx::Log::GetAppLogger()->trace(__VA_ARGS__)
#define LX_INFO(...)			Lynx::Log::GetAppLogger()->info(__VA_ARGS__)
#define LX_WARN(...)			Lynx::Log::GetAppLogger()->warn(__VA_ARGS__)
#define LX_ERROR(...)			Lynx::Log::GetAppLogger()->error(__VA_ARGS__)
#define LX_CRITICAL(...)		Lynx::Log::GetAppLogger()->critical(__VA_ARGS__)