#pragma once
#include <memory>

#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/* Windows x64  */
		#define LX_PLATFORM_WINDOWS
	#else
		/* Windows x86 */
		#error "x86 Builds are not supported!"
	#endif
#else
	/* Unknown compiler/platform */
	#error "Unknown platform!"
#endif

#ifdef LX_DEBUG
	#define LX_ENABLE_ASSERTS
#endif

#ifdef LX_ENABLE_ASSERTS
	#define LX_ASSERT(x, ...) {if(!(x)) {LX_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
	#define LX_CORE_ASSERT(x, ...) {if(!(x)) {LX_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define LX_ASSERT(x, ...)
	#define LX_CORE_ASSERT(x, ...)
#endif

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Lynx {
	template<typename T>
	using Unique = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Unique<T> CreateUnique(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}