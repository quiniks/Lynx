#include "Lynxpch.h"
#include "VoxelRenderer.h"
#include "Lynx/Utility/Packing.h"
#include "Lynx/Utility/Util.h"
#include <glad/glad.h>

namespace Lynx {
	ChunkMesh::ChunkMesh(const glm::uvec3& pos)
	{
		m_Position = { pos.x * Chunk::SIZE * Voxel::SIZE, pos.y * Chunk::SIZE * Voxel::SIZE, pos.z * Chunk::SIZE * Voxel::SIZE };
		m_VA = VertexArray::Create();
		m_VB = VertexBuffer::Create(nullptr, 0);
		m_VB->SetLayout({
			{ Lynx::ShaderDataType::Float3, "a_Position" },
			{ Lynx::ShaderDataType::PackedInt, "a_Color", true },
			{ Lynx::ShaderDataType::UInt, "a_SideAndAO" }
			});
		m_VA->AddVertexBuffer(m_VB);
	}

	void ChunkMesh::Render()
	{
		m_VA->Bind();
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_ChunkVertexData.size());
	}

	void ChunkMesh::ReMesh(const Chunk& chunk)
	{
		glm::vec3 color = glm::vec3{ 1.0f,  1.0f,  1.0f };
		m_ChunkVertexData.clear();

		auto createMesh = [&](const glm::uvec3& vPos) {
			glm::ivec3 ivPos = vPos;
			int vIndex = Chunk::VoxelIndexFromPos(vPos);
			Voxel voxel = chunk.m_Voxels.at(vIndex);

			if (voxel.GetType() != Voxel::Type::Empty) {
				if (chunk.GetVoxelTypeAt(ivPos + glm::ivec3{-1, 0, 0}) == Voxel::Type::Empty) {
					glm::uvec3 vert[4] = { { 0, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 1, 1 } };
					unsigned int ao[4] = {
						CalcAO(chunk, { ivPos.x - 1, ivPos.y, ivPos.z }, { 0, -1, 0 }, { 0, 0, -1 }),
						CalcAO(chunk, { ivPos.x - 1, ivPos.y, ivPos.z }, { 0,  1, 0 }, { 0, 0, -1 }),
						CalcAO(chunk, { ivPos.x - 1, ivPos.y, ivPos.z }, { 0, -1, 0 }, { 0, 0,  1 }),
						CalcAO(chunk, { ivPos.x - 1, ivPos.y, ivPos.z }, { 0,  1, 0 }, { 0, 0,  1 }),
					};
					AddFace(vert, ao, vPos, Chunk::Direction::NX, voxel.GetColor());
				}

				if (chunk.GetVoxelTypeAt(ivPos + glm::ivec3{ 1, 0, 0 }) == Voxel::Type::Empty) {
					glm::uvec3 vert[4] = { { 1, 1, 1 }, { 1, 1, 0 }, { 1, 0, 1 }, { 1, 0, 0 } };
					unsigned int ao[4] = {
						CalcAO(chunk, { ivPos.x + 1, ivPos.y, ivPos.z }, { 0,  1, 0 }, { 0, 0,  1 }),
						CalcAO(chunk, { ivPos.x + 1, ivPos.y, ivPos.z }, { 0,  1, 0 }, { 0, 0, -1 }),
						CalcAO(chunk, { ivPos.x + 1, ivPos.y, ivPos.z }, { 0, -1, 0 }, { 0, 0,  1 }),
						CalcAO(chunk, { ivPos.x + 1, ivPos.y, ivPos.z }, { 0, -1, 0 }, { 0, 0, -1 }),
					};
					AddFace(vert, ao, vPos, Chunk::Direction::PX, voxel.GetColor());
				}

				if (chunk.GetVoxelTypeAt(ivPos + glm::ivec3{ 0, -1, 0 }) == Voxel::Type::Empty) {
					glm::uvec3 vert[4] = { {1, 0, 1},{1, 0, 0},{0, 0, 1},{0, 0, 0} };
					unsigned int ao[4] = {
						CalcAO(chunk, { ivPos.x, ivPos.y - 1, ivPos.z }, {  1, 0, 0 }, { 0, 0,  1 }),
						CalcAO(chunk, { ivPos.x, ivPos.y - 1, ivPos.z }, {  1, 0, 0 }, { 0, 0, -1 }),
						CalcAO(chunk, { ivPos.x, ivPos.y - 1, ivPos.z }, { -1, 0, 0 }, { 0, 0,  1 }),
						CalcAO(chunk, { ivPos.x, ivPos.y - 1, ivPos.z }, { -1, 0, 0 }, { 0, 0, -1 }),
					};
					AddFace(vert, ao, vPos, Chunk::Direction::NY, voxel.GetColor());
				}

				if (chunk.GetVoxelTypeAt(ivPos + glm::ivec3{ 0, 1, 0 }) == Voxel::Type::Empty) {
					glm::uvec3 vert[4] = { {0, 1, 0},{1, 1, 0},{0, 1, 1},{1, 1, 1} };
					unsigned int ao[4] = {
						CalcAO(chunk, { ivPos.x, ivPos.y + 1, ivPos.z }, { -1, 0, 0 }, { 0, 0, -1 }),
						CalcAO(chunk, { ivPos.x, ivPos.y + 1, ivPos.z }, {  1, 0, 0 }, { 0, 0, -1 }),
						CalcAO(chunk, { ivPos.x, ivPos.y + 1, ivPos.z }, { -1, 0, 0 }, { 0, 0,  1 }),
						CalcAO(chunk, { ivPos.x, ivPos.y + 1, ivPos.z }, {  1, 0, 0 }, { 0, 0,  1 }),
					};
					AddFace(vert, ao, vPos, Chunk::Direction::PY, voxel.GetColor());
				}

				if (chunk.GetVoxelTypeAt(ivPos + glm::ivec3{ 0, 0, -1 }) == Voxel::Type::Empty) {
					glm::uvec3 vert[4] = { {0, 0, 0},{1, 0, 0},{0, 1, 0},{1, 1, 0} };
					unsigned int ao[4] = {
						CalcAO(chunk, { ivPos.x, ivPos.y, ivPos.z - 1 }, { -1, 0, 0 }, { 0, -1, 0 }),
						CalcAO(chunk, { ivPos.x, ivPos.y, ivPos.z - 1 }, {  1, 0, 0 }, { 0, -1, 0 }),
						CalcAO(chunk, { ivPos.x, ivPos.y, ivPos.z - 1 }, { -1, 0, 0 }, { 0,  1, 0 }),
						CalcAO(chunk, { ivPos.x, ivPos.y, ivPos.z - 1 }, {  1, 0, 0 }, { 0,  1, 0 }),
					};
					AddFace(vert, ao, vPos, Chunk::Direction::NZ, voxel.GetColor());
				}

				if (chunk.GetVoxelTypeAt(ivPos + glm::ivec3{ 0, 0, 1 }) == Voxel::Type::Empty) {
					glm::uvec3 vert[4] = { {1, 1, 1},{1, 0, 1},{0, 1, 1},{0, 0, 1} };
					unsigned int ao[4] = {
						CalcAO(chunk, { ivPos.x, ivPos.y, ivPos.z + 1 }, {  1, 0, 0 }, { 0,  1, 0 }),
						CalcAO(chunk, { ivPos.x, ivPos.y, ivPos.z + 1 }, {  1, 0, 0 }, { 0, -1, 0 }),
						CalcAO(chunk, { ivPos.x, ivPos.y, ivPos.z + 1 }, { -1, 0, 0 }, { 0,  1, 0 }),
						CalcAO(chunk, { ivPos.x, ivPos.y, ivPos.z + 1 }, { -1, 0, 0 }, { 0, -1, 0 }),
					};
					AddFace(vert, ao, vPos, Chunk::Direction::PZ, voxel.GetColor());
				}
			}
		};
		LoopXYZ(createMesh, { Chunk::SIZE, Chunk::SIZE, Chunk::SIZE });

		m_VB->SetData(m_ChunkVertexData.data(), m_ChunkVertexData.size() * sizeof(ChunkVertex));
	}

	unsigned int ChunkMesh::CalcAO(const Chunk& chunk, const glm::ivec3& p, const glm::ivec3& d1, const glm::ivec3& d2)
	{
		bool adjA2 = false, adjB2 = false, adjC2 = false;
		glm::ivec3 a = p + d1;
		Voxel::Type typeA2 = chunk.GetVoxelTypeAt(a);
		if (typeA2 != Voxel::Type::Empty)
			adjA2 = true;
		glm::ivec3 b = p + d2;
		Voxel::Type typeB2 = chunk.GetVoxelTypeAt(b);
		if (typeB2 != Voxel::Type::Empty)
			adjB2 = true;
		glm::ivec3 c = p + d1 + d2;
		Voxel::Type typeC2 = chunk.GetVoxelTypeAt(c);
		if (typeC2 != Voxel::Type::Empty)
			adjC2 = true;

		if (adjA2 && adjB2)
			return 5;
		return adjA2 + adjB2 + adjC2;
	}

	void ChunkMesh::AddFace(const glm::uvec3* vertices, const unsigned int* ao, const glm::uvec3& p, Chunk::Direction dir, uint32_t color)
	{
		unsigned int side = (unsigned int)dir;
		if (ao[0] + ao[3] > ao[2] + ao[1]) {
			m_ChunkVertexData.emplace_back(ChunkVertex{ glm::vec3{ vertices[0].x + p.x , vertices[0].y + p.y , vertices[0].z + p.z } * Voxel::SIZE + m_Position, color, pack2x4(side, ao[0]) });
			m_ChunkVertexData.emplace_back(ChunkVertex{ glm::vec3{ vertices[2].x + p.x , vertices[2].y + p.y , vertices[2].z + p.z } * Voxel::SIZE + m_Position, color, pack2x4(side, ao[2]) });
			m_ChunkVertexData.emplace_back(ChunkVertex{ glm::vec3{ vertices[1].x + p.x , vertices[1].y + p.y , vertices[1].z + p.z } * Voxel::SIZE + m_Position, color, pack2x4(side, ao[1]) });
																																												
			m_ChunkVertexData.emplace_back(ChunkVertex{ glm::vec3{ vertices[1].x + p.x , vertices[1].y + p.y , vertices[1].z + p.z } * Voxel::SIZE + m_Position, color, pack2x4(side, ao[1]) });
			m_ChunkVertexData.emplace_back(ChunkVertex{ glm::vec3{ vertices[2].x + p.x , vertices[2].y + p.y , vertices[2].z + p.z } * Voxel::SIZE + m_Position, color, pack2x4(side, ao[2]) });
			m_ChunkVertexData.emplace_back(ChunkVertex{ glm::vec3{ vertices[3].x + p.x , vertices[3].y + p.y , vertices[3].z + p.z } * Voxel::SIZE + m_Position, color, pack2x4(side, ao[3]) });
		}																																										
		else {																																									
			m_ChunkVertexData.emplace_back(ChunkVertex{ glm::vec3{ vertices[3].x + p.x , vertices[3].y + p.y , vertices[3].z + p.z } * Voxel::SIZE + m_Position, color, pack2x4(side, ao[3]) });
			m_ChunkVertexData.emplace_back(ChunkVertex{ glm::vec3{ vertices[0].x + p.x , vertices[0].y + p.y , vertices[0].z + p.z } * Voxel::SIZE + m_Position, color, pack2x4(side, ao[0]) });
			m_ChunkVertexData.emplace_back(ChunkVertex{ glm::vec3{ vertices[2].x + p.x , vertices[2].y + p.y , vertices[2].z + p.z } * Voxel::SIZE + m_Position, color, pack2x4(side, ao[2]) });
																																												
			m_ChunkVertexData.emplace_back(ChunkVertex{ glm::vec3{ vertices[3].x + p.x , vertices[3].y + p.y , vertices[3].z + p.z } * Voxel::SIZE + m_Position, color, pack2x4(side, ao[3]) });
			m_ChunkVertexData.emplace_back(ChunkVertex{ glm::vec3{ vertices[1].x + p.x , vertices[1].y + p.y , vertices[1].z + p.z } * Voxel::SIZE + m_Position, color, pack2x4(side, ao[1]) });
			m_ChunkVertexData.emplace_back(ChunkVertex{ glm::vec3{ vertices[0].x + p.x , vertices[0].y + p.y , vertices[0].z + p.z } * Voxel::SIZE + m_Position, color, pack2x4(side, ao[0]) });
		}
	}

	void VoxelRenderer::Init(World& world)
	{
		unsigned int chunkCount = world.m_Chunks.size();
		m_Meshes.reserve(chunkCount);
		for (auto chunk : world.m_Chunks) {
			ChunkMesh mesh{ chunk->m_ChunkPosition };
			m_Meshes.emplace_back(mesh);
			world.m_DirtyChunkPositions.push_back(chunk->m_ChunkPosition);
		}
	}

	void VoxelRenderer::Render(World& world)
	{
		for (auto& mesh : m_Meshes) {
			mesh.Render();
		}
	}

	void VoxelRenderer::Update(World& world)
	{
		for (auto pos : world.m_DirtyChunkPositions) {
			unsigned int index = world.ChunkIndexFromPos(pos);
			if (index < m_Meshes.size() && world.ValidChunkPos(pos)) {
				ChunkMesh& mesh = m_Meshes.at(index);
				mesh.ReMesh(*world.m_Chunks.at(index));
			}
		}
		world.m_DirtyChunkPositions.clear();
	}
}