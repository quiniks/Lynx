#pragma once

#include <iostream>
#include <sstream>
#include <memory>
#include <functional>
#include <algorithm>
#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Lynx/Core/Core.h"
#include "Lynx/Core/Log.h"
#include "Lynx/Utility/Instrumentor.h"

#ifdef LX_PLATFORM_WINDOWS
	#include <Windows.h>
#endif