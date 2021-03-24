/*
#pragma once
#include "Lynx/Voxel/Voxel.h"
#include "Lynx/Render/Buffer.h"
#include "Lynx/Render/VertexArray.h"

namespace Lynx {
	enum Direction {
		Up, North, East, South, West, Down, Total
	};

	struct VertexData {
		glm::vec3 Pos;
		uint32_t Color;
		uint8_t SideAndAO;
	};

	class Chunk {
	public:
		Chunk(const glm::uvec3& chunkPos);
		void Mesh();
		////
		static const unsigned int SIZE = 16;
		glm::uvec3 m_ChunkPosition{ 0 };
		glm::vec3 m_ChunkModelPos{ 0.0f };
		Ref<Chunk> m_AdjChunks[Direction::Total] = { nullptr };
		std::vector<Voxel2> m_Voxels[SIZE * SIZE * SIZE];
		////
		std::vector<VertexData> m_VertexData;
		Ref<VertexArray> m_VA = nullptr;
		Ref<VertexBuffer> m_VB = nullptr;
	};


	class World {
	public:
		void Load();
		void Mesh();
		////
		glm::uvec3 m_WorldSize{ 0 };
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
*/