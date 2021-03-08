#include "Lynxpch.h"
#include "World.h"
#include "Lynx/Voxel/Voxel.h"
#include "Lynx/Events/Input.h"
#include "Lynx/Voxel/VoxelRay.h"
#include <glad/glad.h>

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
					m_Chunks.at(IndexLinear(x, y, z)).CreateMesh(x * Chunk::SIZE * Voxel::SIZE, y * Chunk::SIZE * Voxel::SIZE, z * Chunk::SIZE * Voxel::SIZE);
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

	bool World::VoxelPick(const FreeCamera& camera, glm::ivec3& voxelPosOut)
	{
		GLint viewportSize[4];
		glGetIntegerv(GL_VIEWPORT, viewportSize);
		float x = (2.0f * Lynx::Input::GetMouseX()) / viewportSize[2] - 1.0f;
		float y = 1.0f - (2.0f * Lynx::Input::GetMouseY()) / viewportSize[3];
		glm::vec4 rayEye = glm::inverse(camera.GetProjection()) * glm::vec4(x, y, -1.0f, 1.0);
		glm::vec3 rayWorld = (glm::inverse(camera.GetView()) * glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0f));
		rayWorld = glm::normalize(rayWorld);
		////////////

		std::vector<glm::ivec3> voxelList2;
		VoxelRayData rayData2;
		rayData2.origin = camera.GetPosition();
		rayData2.direction = rayWorld;
		rayData2.maxDistance = 50.0f;
		rayData2.unitSize = Voxel::SIZE;
		rayData2.offset = { 0.0f, 0.0f, 0.0f };// glm::vec3{ Voxel::SIZE / 2.0f };
		VoxelRay::PosFromRay(rayData2, voxelList2);
		for (auto i = voxelList2.begin(); i != voxelList2.end(); ++i) {
			glm::ivec3 voxelPos2 = *i;
			glm::ivec3 chunkPos = glm::floor((glm::vec3)voxelPos2 / (float)Chunk::SIZE);
			glm::ivec3 voxelPosChunkSpace = glm::mod((glm::vec3)voxelPos2, (float)Chunk::SIZE);
			if (Inside(chunkPos.x, chunkPos.y, chunkPos.z)) {
				Chunk& chunk = GetChunk(chunkPos.x, chunkPos.y, chunkPos.z);
				Voxel::Type voxel = chunk.GetVoxel(voxelPosChunkSpace.x, voxelPosChunkSpace.y, voxelPosChunkSpace.z);
				if (voxel != Voxel::Type::Empty) {
					voxelPosOut = voxelPos2;
					return true;
				}
			}
		}
		return false;


		//////////
		/*
		std::vector<glm::ivec3> chunkList;
		VoxelRayData rayData;
		rayData.origin = camera.GetPosition();
		rayData.direction = rayWorld;
		rayData.maxDistance = 50.0f;
		rayData.unitSize = Chunk::SIZE * Voxel::SIZE;
		rayData.offset = { 0.0f, 0.0f, 0.0f };// glm::vec3{ Voxel::SIZE / 2.0f };
		VoxelRay::PosFromRay(rayData, chunkList);

		for (auto i = chunkList.begin(); i != chunkList.end(); ++i) {
			glm::ivec3 chunkPos = *i;
			if (Inside(chunkPos.x, chunkPos.y, chunkPos.z)) {
				Chunk& chunk = GetChunk(chunkPos.x, chunkPos.y, chunkPos.z);
				std::vector<glm::ivec3> voxelList;
				rayData.unitSize = Voxel::SIZE;
				rayData.offset = -(glm::vec3)chunkPos * (float)Chunk::SIZE * Voxel::SIZE;// glm::vec3{ Voxel::SIZE / 2.0f } - (glm::vec3)chunkPos * (float)Chunk::SIZE * Voxel::SIZE;
				VoxelRay::PosFromRay(rayData, voxelList);

				for (auto i = voxelList.begin(); i != voxelList.end(); ++i) {
					glm::ivec3 voxelPos = *i;
					if (Chunk::Inside(voxelPos.x, voxelPos.y, voxelPos.z)) {
						Voxel::Type voxel = chunk.GetVoxel(voxelPos.x, voxelPos.y, voxelPos.z);
						if (voxel != Voxel::Type::Empty) {
							voxelPosOut = voxelPos + chunkPos * Chunk::SIZE;
							return true;
						}
					}
				}
			}
		}
		return false;
		*/
	}

	void World::VoxelSet(const glm::ivec3& voxelPos, Voxel::Type type)
	{
		glm::ivec3 chunkPos = glm::floor((glm::vec3)voxelPos / (float)Chunk::SIZE);
		//LX_INFO("chunkPos: {0}, {1}, {2}", chunkPos.x, chunkPos.y, chunkPos.z);
		glm::ivec3 voxelPosChunkSpace = glm::mod((glm::vec3)voxelPos, (float)Chunk::SIZE);
		//LX_INFO("voxelPosChunkSpace: {0}, {1}, {2}", voxelPosChunkSpace.x, voxelPosChunkSpace.y, voxelPosChunkSpace.z);
		Chunk& chunk = GetChunk(chunkPos.x, chunkPos.y, chunkPos.z);
		Voxel::Type& voxel = chunk.GetVoxel(voxelPosChunkSpace.x, voxelPosChunkSpace.y, voxelPosChunkSpace.z);
		voxel = type;
		chunk.Update();
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
	float World::TAtBound(float origin, float direction)
	{
		////Solve o+td=1 in each dimension
		float distToBoundTowardDir = 0.0f;
		distToBoundTowardDir = direction > 0.0f ? ceilf(origin) - origin : origin - floorf(origin);
		return distToBoundTowardDir / abs(direction);
	}
}