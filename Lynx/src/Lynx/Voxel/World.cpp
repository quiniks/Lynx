#include "Lynxpch.h"
#include "World.h"
#include "Lynx/Voxel/Voxel.h"

namespace Lynx {
	World::World()
	{
		m_Chunks.reserve((size_t)SIZE.x * SIZE.y * SIZE.z);
		for (int x = 0; x < SIZE.x; x++) {
			for (int y = 0; y < SIZE.y; y++) {
				for (int z = 0; z < SIZE.z; z++) {
					m_Chunks.emplace_back(*this, x, y, z);
				}
			}
		}

		for (int x = 0; x < SIZE.x; x++) {
			for (int y = 0; y < SIZE.y; y++) {
				for (int z = 0; z < SIZE.z; z++) {
					m_Chunks.at(IndexLinear(x, y, z)).CreateMesh(x * Chunk::SIZE * Voxel::SIZE * 1.1f, y * Chunk::SIZE * Voxel::SIZE * 1.1f, z * Chunk::SIZE * Voxel::SIZE * 1.1f);
				}
			}
		}
	}

	void World::Render()
	{

		for (auto i = m_Chunks.begin(); i != m_Chunks.end(); ++i) {
			(*i).Render();
		}
	}

	Chunk& World::GetChunk(int x, int y, int z)
	{
		if (!Inside(x, y, z))
			LX_CORE_ASSERT(false, "Chunk is not inside world");
		return m_Chunks.at(IndexLinear(x, y, z));
	}

	int World::IndexLinear(int x, int y, int z)
	{
		int a = SIZE.z * SIZE.y;	//Z * Y
		int b = SIZE.z;			//Z
		return a * x + b * y + z;
	}

	bool World::Inside(int x, int y, int z)
	{
		bool upper = x >= SIZE.x || y >= SIZE.y || z >= SIZE.z;
		bool lower = x < 0.0f || y < 0.0f || z < 0.0f;
		if (upper || lower)
			return false;
		return true;
	}
}