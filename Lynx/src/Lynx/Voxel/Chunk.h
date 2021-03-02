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
		glm::vec3 Normal;
	};

	class Chunk {
	public:
		Chunk(World& world, int x, int y, int z);

		Voxel::Type& GetVoxel(int x, int y, int z);

		static int IndexLinear(int x, int y, int z);
		static int VIndexLinear(int x, int y, int z);
		static bool Inside(int x, int y, int z);
		static const int SIZE = 3;

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
		int AddVertex(const glm::vec3& pos, const glm::vec3& color, const glm::vec3& normal);
		glm::vec3 m_Position{ 0.0f };
		glm::ivec3 m_ChunkPosition{ 0 };
		std::vector<VertexData> m_VertexData;
		Ref<VertexArray> m_VA = nullptr;
		Ref<VertexBuffer> m_VB = nullptr;

		int m_Count = 0;
	};
}