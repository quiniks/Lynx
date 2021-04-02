#pragma once
#include "Lynx/Voxel/VoxelStructure.h"
#include "Lynx/Render/VertexArray.h"
#include "Lynx/Render/Buffer.h"
#include "Lynx/Detail/glm.h"

namespace Lynx {
	struct ChunkVertex {
		glm::vec3 Pos;
		uint32_t Color;
		uint8_t SideAndAO;
	};

	class ChunkMesh {
	public:
		ChunkMesh(const glm::uvec3& pos);
		void Render();
		void ReMesh(const Chunk& chunk);
	private:
		int CalcAO(const Chunk& chunk, const glm::ivec3& p, const glm::ivec3& d1, const glm::ivec3& d2);
		void AddFace(const glm::uvec3* vertices, const unsigned int* ao, const glm::uvec3& p, Chunk::Direction dir, uint32_t color);
	private:
		glm::vec3 m_Position{ 0.0f };
		std::vector<ChunkVertex> m_ChunkVertexData;
		Ref<VertexArray> m_VA = nullptr;
		Ref<VertexBuffer> m_VB = nullptr;
	};

	class VoxelRenderer {
	public:
		void Init(World& world);
		void Render(World& world);
		void Update(World& world);
	private:
		std::vector<ChunkMesh> m_Meshes;
	};
}
