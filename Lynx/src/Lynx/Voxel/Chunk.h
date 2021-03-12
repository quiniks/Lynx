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
		int Side;
		int AO;
	};

	class Chunk {
	public:
		Chunk(World& world, int x, int y, int z);
		void CreateMesh(float x, float y, float z);

		Voxel::Type GetVoxel(int x, int y, int z);
		void SetVoxel(int x, int y, int z, Voxel::Type type);

		void Update();
		void Render();

		static int IndexLinear(int x, int y, int z);
		static int IndexLinear(const glm::ivec3& p);
		static bool Inside(int x, int y, int z);
		static bool Inside(const glm::ivec3& p);
		static const int SIZE = 16;
	private:

		std::vector<Voxel::Type> m_Voxels;
		World& m_World;

		glm::vec3 m_Position{ 0.0f };
		glm::ivec3 m_ChunkPosition{ 0 };

		//Mesh data
		void CreateVoxelData();
		void AddVertex(const glm::vec3& pos, const glm::vec3& color, int side, int ao);
		void AddFace(const glm::ivec3* vertices, const int* ao, const glm::ivec3& p, const glm::ivec3& d, const glm::vec3& color);
		int VertexAO(const glm::ivec3& p, const glm::ivec3& d1, const glm::ivec3& d2);

		std::vector<VertexData> m_VertexData;
		Ref<VertexArray> m_VA = nullptr;
		Ref<VertexBuffer> m_VB = nullptr;

		int m_Count = 0;
	};
}