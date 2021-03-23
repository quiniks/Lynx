#pragma once
#include "Lynx/Voxel/Voxel.h"

namespace Lynx {
	enum Direction {
		Up, North, East, South, West, Down, Total
	};

	class Chunk {
	public:
		Chunk(const glm::uvec3& chunkPos);

		static const unsigned int SIZE = 16;
		glm::uvec3 m_ChunkPosition{ 0 };
		Ref<Chunk> m_AdjChunks[Direction::Total] = { nullptr };
		std::vector<Voxel2> m_Voxels[SIZE * SIZE * SIZE];
	};

	class World {
	public:
		void Load();
		std::unordered_map<glm::uvec3, Ref<Chunk>> m_Chunks;
	};

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