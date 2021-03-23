#include "Lynxpch.h"
#include "Lynx/Voxel/WorldTest.h"

namespace Lynx {
	Chunk::Chunk(const glm::uvec3& chunkPos) : m_ChunkPosition(chunkPos)
	{
		
	}

	void World::Load()
	{
		glm::uvec3 tempSize = { 2, 1, 2 };

		auto createChunks = [&](const glm::uvec3& chunkPos) {
			m_Chunks.insert({ chunkPos, CreateUnique<Chunk>() });
		};
		LoopXYZ(createChunks, tempSize);

		auto connectChunks = [&](const glm::uvec3& chunkPos) {
			auto& pairA = m_Chunks.find(chunkPos);
			Chunk& chunk = *pairA->second;

			auto connect = [&](const glm::uvec3& dirA, Direction dirB) {
				auto& pairB = m_Chunks.find(dirA);
				if (pairB != m_Chunks.end()) {
					chunk.m_AdjChunks[dirB] = pairB->second;
				}
			};

			connect(chunkPos + glm::uvec3{	0,	1,	0 }, Direction::Up);
			connect(chunkPos + glm::uvec3{	0,	0,	1 }, Direction::North);
			connect(chunkPos + glm::uvec3{	1,	0,	0 }, Direction::East);
			connect(chunkPos + glm::uvec3{	0,	0, -1 }, Direction::South);
			connect(chunkPos + glm::uvec3{ -1,	0,	0 }, Direction::West);
			connect(chunkPos + glm::uvec3{	0, -1,	0 }, Direction::Down);
		};
		LoopXYZ(connectChunks, tempSize);
	}
}