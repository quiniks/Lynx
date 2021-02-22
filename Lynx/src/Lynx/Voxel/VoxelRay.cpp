#include "Lynxpch.h"
#include "VoxelRay.h"

void Lynx::VoxelRay::PosFromRay(glm::vec3 origin, const glm::vec3& direction, float maxDistance, std::vector<glm::ivec3>& positions)
{
	float voxelSize = 0.2f;
	float offset = -5.0f;
	origin = (origin + voxelSize / 2.0f - offset) * 1.0f/ voxelSize;

	glm::vec3 pos = glm::floor(origin);
	glm::vec3 step = glm::sign(direction);

	if (step.x == 0.0f && step.y == 0.0f && step.z == 0.0f)
		LX_CORE_ASSERT(true, "Cannot cast ray with direction of { 0, 0, 0 }");

	glm::vec3 tMax{ 0.0f };
	glm::vec3 tDelta{ 0.0f };

	glm::bvec3 nonZeroDir{ false };

	if (step.x != 0.0f) {
		nonZeroDir.x = true;
		tMax.x = VoxelRay::TAtBound(origin.x, direction.x);
		tDelta.x = step.x / direction.x;
	}
	if (step.y != 0.0f) {
		nonZeroDir.y = true;
		tMax.y = VoxelRay::TAtBound(origin.y, direction.y);
		tDelta.y = step.y / direction.y;
	}
	if (step.z != 0.0f) {
		nonZeroDir.z = true;
		tMax.z = VoxelRay::TAtBound(origin.z, direction.z);
		tDelta.z = step.z / direction.z;
	}

	glm::vec3 dist(0.0f);
	glm::vec3 originPos = pos;

	while (dist.x < maxDistance && dist.y < maxDistance && dist.z < maxDistance) {
		
		dist = glm::abs(pos - originPos);
		positions.emplace_back(pos.x, pos.y, pos.z);

		if ((nonZeroDir.x || nonZeroDir.z) && tMax.x < tMax.y) {
			if (nonZeroDir.x && tMax.x < tMax.z) {
				pos.x += step.x;
				tMax.x += tDelta.x;
			}
			else {
				pos.z += step.z;
				tMax.z += tDelta.z;
			}
		}
		else {
			if (nonZeroDir.y && tMax.y < tMax.z) {
				pos.y += step.y;
				tMax.y += tDelta.y;
			}
			else {
				pos.z += step.z;
				tMax.z += tDelta.z;
			}
		}
	}
}

float Lynx::VoxelRay::TAtBound(float origin, float direction)
{
	////Solve o+td=1 in each dimension
	float distToBoundTowardDir = 0.0f;
	distToBoundTowardDir = direction > 0.0f ? ceilf(origin) - origin : origin - floorf(origin);
	return distToBoundTowardDir / abs(direction);
}
