#include "Lynxpch.h"
/*
#include "Lynx/Voxel/WorldTest.h"

namespace Lynx {
	Chunk::Chunk(const glm::uvec3& chunkPos) : m_ChunkPosition(chunkPos)
	{
		m_ChunkModelPos = (glm::vec3)chunkPos * (float)Chunk::SIZE * Voxel::SIZE;
	}

	void Chunk::Mesh()
	{

		m_VA = VertexArray::Create();
		m_VB = VertexBuffer::Create(m_VertexData.data(), m_VertexData.size() * sizeof(VertexData));
		m_VB->SetLayout({
			{ Lynx::ShaderDataType::Float3, "a_Position" },
			{ Lynx::ShaderDataType::PackedInt, "a_Color" },
			{ Lynx::ShaderDataType::UInt, "a_SideAndAO" }
			});
		m_VA->AddVertexBuffer(m_VB);
	}

	void World::Load()
	{
		glm::uvec3 tempSize = { 2, 1, 2 };
		//populate chunks
		auto createChunks = [&](const glm::uvec3& chunkPos) {
			Ref<Chunk> chunk = CreateRef<Chunk>();
			m_Chunks.insert({ chunkPos, chunk });
			for (auto& v : *chunk->m_Voxels) {
				v.m_Type = Voxel::Type::Solid;
			}
		};
		LoopXYZ(createChunks, tempSize);
		//connect chunks
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

	void World::Mesh()
	{
		auto meshify = [&](const glm::uvec3& chunkPos) {
			for (auto& chunk : m_Chunks) {
				chunk.second->Mesh();
			}
		};
		LoopXYZ(meshify, m_WorldSize);
	}
}
*/