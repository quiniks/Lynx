#pragma once
#include "Lynx/Voxel/Voxel.h"
#include "Lynx/Render/VertexArray.h"
#include "Lynx/Render/Buffer.h"
#include "Lynx/Detail/glm.h"

namespace Lynx {
	class World;

	struct VertexData {
		glm::vec3 Pos;
		glm::vec3 Color;
		int ActiveSides;
	};

	class Chunk {
	public:
		Chunk(World& world, int x, int y, int z);

		Voxel::Type& GetVoxel(int x, int y, int z);

		static int IndexLinear(int x, int y, int z);
		static bool Inside(int x, int y, int z);
		static const int SIZE = 16;

		//Mesh data
		void Update();
		void Render();
		void CreateMesh(float x, float y, float z);
	private:
		std::vector<Voxel::Type> m_Voxels;
		World& m_World;

		//Mesh data
		void CreateVoxelData();
		int ActiveSidesOfVoxel(int x, int y, int z);
		void AllActiveSides(std::vector<int>&);
		int AdjacentChunkCheckN(float ChunkAxisPos, const glm::ivec3& offset, const glm::ivec3& voxelPos, int side);
		int AdjacentChunkCheckP(float chunkAxisPos, float worldAxisSize, const glm::ivec3& offset, const glm::ivec3& voxelPos, int side);
		glm::vec3 m_Position{ 0.0f };
		glm::ivec3 m_ChunkPosition{ 0 };
		std::vector<VertexData> m_VoxelData;
		Ref<VertexArray> m_VA = nullptr;
		Ref<VertexBuffer> m_VB = nullptr;
	};
}