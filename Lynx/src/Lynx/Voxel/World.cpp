#include "Lynxpch.h"
#include "World.h"
#include "Lynx/Voxel/Voxel.h"
#include "Lynx/Events/Input.h"
#include "Lynx/Voxel/VoxelRay.h"
#include "Lynx/Voxel/Importer.h"
#include <glad/glad.h>

namespace Lynx {
	World::World()
	{
	}

	void World::Init()
	{

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
			//glm::ivec3 voxelPosChunkSpace = glm::mod(voxelPos2, Chunk::SIZE);
			int vcpX = voxelPos2.x % Chunk::SIZE;
			int vcpY = voxelPos2.y % Chunk::SIZE;
			int vcpZ = voxelPos2.z % Chunk::SIZE;
			if (Inside(chunkPos.x, chunkPos.y, chunkPos.z)) {
				Chunk& chunk = GetChunk(chunkPos.x, chunkPos.y, chunkPos.z);
				Voxel2 voxel = chunk.GetVoxel(vcpX, vcpY, vcpZ);
				if (voxel.m_Type != Voxel::Type::Empty) {
					voxelPosOut = voxelPos2;
					return true;
				}
			}
		}
		return false;
	}

	void World::VoxelSet(const glm::ivec3& voxelPos, Voxel::Type type)
	{
		glm::ivec3 chunkPos = glm::floor((glm::vec3)voxelPos / (float)Chunk::SIZE);
		//glm::ivec3 voxelPosChunkSpace = glm::mod(voxelPos, Chunk::SIZE);
		int vcpX = voxelPos.x % Chunk::SIZE;
		int vcpY = voxelPos.y % Chunk::SIZE;
		int vcpZ = voxelPos.z % Chunk::SIZE;
		Chunk& chunk = GetChunk(chunkPos.x, chunkPos.y, chunkPos.z);
		chunk.SetVoxelType(vcpX, vcpY, vcpZ, type);
		chunk.Update();
	}

	void World::Load(const std::string& file)
	{
		Lynx::XRAW xraw = Lynx::Importer::XRawImport(file);
		m_Chunks = Lynx::Importer::XRAWToVoxel(xraw, *this);
		MakeMesh();
	}

	void World::MakeMesh()
	{
		for (auto& chunk : m_Chunks) {
			chunk.CreateMesh();
		}
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