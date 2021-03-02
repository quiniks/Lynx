#pragma once
#include "Lynx/Detail/glm.h"

namespace Lynx {
	struct VoxelRayData {
		glm::vec3 origin;
		glm::vec3 direction;
		float maxDistance;
		float unitSize;
		glm::vec3 offset;
	};

	class VoxelRay {
	public:
		static void PosFromRay(const VoxelRayData& rayData, std::vector<glm::ivec3>& positions);
	private:
		static float TAtBound(float origin, float direction);
	};
}

