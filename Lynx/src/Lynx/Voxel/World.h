#pragma once
#include "Lynx/Voxel/Chunk.h"

namespace Lynx {
	class World {
	public:
		World();
		void Render();
		Chunk& GetChunk(int x, int y, int z);

		static int IndexLinear(int x, int y, int z);
		static bool Inside(int x, int y, int z);
		static constexpr glm::ivec3 SIZE{ 5, 1, 5 };
	private:
		std::vector<Chunk> m_Chunks;
	};
}