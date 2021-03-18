#pragma once
#include "Lynx/Voxel/Chunk.h"
#include "Lynx/Render/FreeCamera.h"

namespace Lynx {
	class World {
	public:
		World();
		void Init();
		void Render();
		Chunk& GetChunk(int x, int y, int z);
		bool VoxelPick(const FreeCamera& camera, glm::ivec3& voxelPosOut);
		void VoxelSet(const glm::ivec3& voxelPos, Voxel::Type type);

		static int IndexLinear(int x, int y, int z);
		static bool Inside(int x, int y, int z);
		static constexpr glm::ivec3 SIZE{ 10, 2, 10 };
	private:
		std::vector<Chunk> m_Chunks;
	};
}