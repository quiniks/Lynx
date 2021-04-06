#include "Lynxpch.h"
#include "Lynx/Voxel/VoxelStructure.h"
#include "Lynx/Events/Input.h"
#include "Lynx/Voxel/VoxelRay.h"
#include "Lynx/Voxel/Importer.h"
#include "Lynx/Utility/Util.h"
#include <glad/glad.h>

namespace Lynx {
	Voxel::Type Chunk::GetVoxelTypeAt(const glm::ivec3& vLocalPos) const
	{
		if (vLocalPos.x < 0) {
			if (m_AdjChunks[(unsigned)Chunk::Direction::NX] != nullptr)
				return m_AdjChunks[(unsigned)Chunk::Direction::NX]->m_Voxels.at(VoxelIndexFromPos({ Chunk::SIZE - 1, vLocalPos.y, vLocalPos.z })).GetType();
			else
				return Voxel::Type::Empty;
		}
		else if (vLocalPos.x > Chunk::SIZE - 1) {
			if (m_AdjChunks[(unsigned)Chunk::Direction::PX] != nullptr)
				return m_AdjChunks[(unsigned)Chunk::Direction::PX]->m_Voxels.at(VoxelIndexFromPos({ 0, vLocalPos.y, vLocalPos.z })).GetType();
			else
				return Voxel::Type::Empty;
		}

		if (vLocalPos.y < 0) {
			if (m_AdjChunks[(unsigned)Chunk::Direction::NY] != nullptr)
				return m_AdjChunks[(unsigned)Chunk::Direction::NY]->m_Voxels.at(VoxelIndexFromPos({ vLocalPos.x, Chunk::SIZE - 1, vLocalPos.z })).GetType();
			else
				return Voxel::Type::Empty;
		}
		else if (vLocalPos.y > Chunk::SIZE - 1) {
			if (m_AdjChunks[(unsigned)Chunk::Direction::PY] != nullptr)
				return m_AdjChunks[(unsigned)Chunk::Direction::PY]->m_Voxels.at(VoxelIndexFromPos({ vLocalPos.x, 0, vLocalPos.z })).GetType();
			else
				return Voxel::Type::Empty;
		}

		if (vLocalPos.z < 0) {
			if (m_AdjChunks[(unsigned)Chunk::Direction::NZ] != nullptr)
				return m_AdjChunks[(unsigned)Chunk::Direction::NZ]->m_Voxels.at(VoxelIndexFromPos({ vLocalPos.x, vLocalPos.y, Chunk::SIZE - 1 })).GetType();
			else
				return Voxel::Type::Empty;
		}
		else if (vLocalPos.z > Chunk::SIZE - 1) {
			if (m_AdjChunks[(unsigned)Chunk::Direction::PZ] != nullptr)
				return m_AdjChunks[(unsigned)Chunk::Direction::PZ]->m_Voxels.at(VoxelIndexFromPos({ vLocalPos.x, vLocalPos.y, 0 })).GetType();
			else
				return Voxel::Type::Empty;
		}

		return m_Voxels.at(Chunk::VoxelIndexFromPos(vLocalPos)).GetType();
	}

	unsigned int Chunk::VoxelIndexFromPos(const glm::uvec3& pos)
	{
		unsigned int a = Chunk::SIZE * Chunk::SIZE;
		unsigned int b = Chunk::SIZE;
		return a * pos.x + b * pos.y + pos.z;
	}

	void World::LoadXRAW(const std::string& file)
	{
		Lynx::XRAW xraw = Lynx::Importer::XRawImport(file);
		Lynx::Importer::XRAWToVoxel(xraw, *this);
	}

	bool World::LookingAtVoxel(const FreeCamera& camera, glm::uvec3& vpos)
	{
		GLint viewportSize[4];
		glGetIntegerv(GL_VIEWPORT, viewportSize);
		float x = (2.0f * Lynx::Input::GetMouseX()) / viewportSize[2] - 1.0f;
		float y = 1.0f - (2.0f * Lynx::Input::GetMouseY()) / viewportSize[3];
		glm::vec4 rayEye = glm::inverse(camera.GetProjection()) * glm::vec4(x, y, -1.0f, 1.0);
		glm::vec3 rayWorld = (glm::inverse(camera.GetView()) * glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0f));
		rayWorld = glm::normalize(rayWorld);

		//////////////////

		std::vector<glm::ivec3> voxelList2;
		VoxelRayData rayData2;
		rayData2.origin = camera.GetPosition();
		rayData2.direction = rayWorld;
		rayData2.maxDistance = 50.0f;
		rayData2.unitSize = Voxel::SIZE;
		rayData2.offset = { 0.0f, 0.0f, 0.0f };
		VoxelRay::PosFromRay(rayData2, voxelList2);

		///////////////////

		for (auto i = voxelList2.begin(); i != voxelList2.end(); ++i) {
			glm::ivec3 voxelPos2 = *i;
			glm::ivec3 chunkPos = glm::floor((glm::vec3)voxelPos2 / (float)Chunk::SIZE);

			if (ValidChunkPos(chunkPos)) {
				glm::ivec3 voxelLocalPos;
				voxelLocalPos.x = voxelPos2.x % Chunk::SIZE;
				voxelLocalPos.y = voxelPos2.y % Chunk::SIZE;
				voxelLocalPos.z = voxelPos2.z % Chunk::SIZE;

				Chunk& chunk = *m_Chunks.at(ChunkIndexFromPos(chunkPos));
				Voxel& voxel = chunk.m_Voxels.at(Chunk::VoxelIndexFromPos(voxelLocalPos));
				if (voxel.GetType() != Voxel::Type::Empty) {
					vpos = voxelPos2;
					return true;
				}
			}
		}
		return false;
	}

	void World::SetVoxel(const glm::uvec3 vpos, Voxel::Type type, const glm::vec4& color)
	{
		glm::uvec3 chunkPos = glm::floor((glm::vec3)vpos / (float)Chunk::SIZE);
		if (ValidChunkPos(chunkPos)) {
			glm::uvec3 voxelLocalPos;
			voxelLocalPos.x = vpos.x % Chunk::SIZE;
			voxelLocalPos.y = vpos.y % Chunk::SIZE;
			voxelLocalPos.z = vpos.z % Chunk::SIZE;

			Voxel& voxel = m_Chunks.at(ChunkIndexFromPos(chunkPos))->m_Voxels.at(Chunk::VoxelIndexFromPos(voxelLocalPos));
			voxel.SetType(type);
			voxel.SetColor(color);

			m_DirtyChunkPositions.emplace_back(chunkPos.x, chunkPos.y, chunkPos.z);

			if (voxelLocalPos.x == 0) {
				m_DirtyChunkPositions.emplace_back(chunkPos.x - 1, chunkPos.y, chunkPos.z);
			}
			else if (voxelLocalPos.x == Chunk::SIZE - 1) {
				m_DirtyChunkPositions.emplace_back(chunkPos.x + 1, chunkPos.y, chunkPos.z);
			}

			if (voxelLocalPos.y == 0) {
				m_DirtyChunkPositions.emplace_back(chunkPos.x, chunkPos.y - 1, chunkPos.z);
			}
			else if (voxelLocalPos.y == Chunk::SIZE - 1) {
				m_DirtyChunkPositions.emplace_back(chunkPos.x, chunkPos.y + 1, chunkPos.z);
			}

			if (voxelLocalPos.z == 0) {
				m_DirtyChunkPositions.emplace_back(chunkPos.x, chunkPos.y, chunkPos.z - 1);
			}
			else if (voxelLocalPos.z == Chunk::SIZE - 1) {
				m_DirtyChunkPositions.emplace_back(chunkPos.x, chunkPos.y, chunkPos.z +1);
			}
		}
	}

	bool World::ValidChunkPos(const glm::uvec3& pos)
	{
		bool upper = pos.x >= m_Size.x || pos.y >= m_Size.y || pos.z >= m_Size.z;
		bool lower = pos.x < 0.0f || pos.y < 0.0f || pos.z < 0.0f;
		if (upper || lower)
			return false;
		return true;
	}

	unsigned int World::ChunkIndexFromPos(const glm::uvec3& pos)
	{
		unsigned int a = m_Size.z * m_Size.y;
		unsigned int b = m_Size.z;
		return a * pos.x + b * pos.y + pos.z;
	}

	void World::connectChunks()
	{
		auto connectChunks = [&](const glm::uvec3& cPos) {
			Chunk& chunkA = *m_Chunks.at(ChunkIndexFromPos(cPos));

			auto connect = [&](const glm::uvec3& cPosAdj, unsigned int index) {
				if (ValidChunkPos(cPosAdj)) {
					Ref<Chunk> chunkB = m_Chunks.at(ChunkIndexFromPos(cPosAdj));
					chunkA.m_AdjChunks[index] = chunkB;
				}
			};

			connect(cPos + glm::uvec3{  0,  1,	0 }, (unsigned)Chunk::Direction::PY);
			connect(cPos + glm::uvec3{  0,  0,	1 }, (unsigned)Chunk::Direction::PZ);
			connect(cPos + glm::uvec3{  1,  0,	0 }, (unsigned)Chunk::Direction::PX);
			connect(cPos + glm::uvec3{  0,  0, -1 }, (unsigned)Chunk::Direction::NZ);
			connect(cPos + glm::uvec3{ -1,  0,	0 }, (unsigned)Chunk::Direction::NX);
			connect(cPos + glm::uvec3{  0, -1,	0 }, (unsigned)Chunk::Direction::NY);
		};
		LoopXYZ(connectChunks, m_Size);
	}
}
