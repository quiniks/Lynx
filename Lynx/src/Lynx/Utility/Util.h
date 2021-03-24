#pragma once
#include "Lynx/Detail/glm.h"
#include <functional>

namespace Lynx {
	void LoopXYZ(const std::function <void(glm::uvec3)>& func, const glm::uvec3 size) {
		for (unsigned int x = 0; x < size.x; ++x) {
			for (unsigned int y = 0; y < size.y; ++y) {
				for (unsigned int z = 0; z < size.z; ++z) {
					func({ x, y, z });
				}
			}
		}
	}
}