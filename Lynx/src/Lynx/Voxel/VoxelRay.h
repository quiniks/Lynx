#pragma once
#include "Lynx/Detail/glm.h"

namespace Lynx {
	class VoxelRay {
	public:
		static void PosFromRay(glm::vec3 origin, const glm::vec3& direction, float maxDistance, std::vector<glm::ivec3>& positions);
	private:
		static float TAtBound(float origin, float direction);
	};
}

