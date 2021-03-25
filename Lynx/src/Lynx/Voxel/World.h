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

		void Load(const std::string& file);
		void MakeMesh();

		int IndexLinear(int x, int y, int z);
		bool Inside(int x, int y, int z);

		std::vector<Chunk>& Chunks() {
			return m_Chunks;
		}
		glm::uvec3 m_Size{ 0 };
	private:
		std::vector<Chunk> m_Chunks;
	};
}