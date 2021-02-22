#pragma once
#include "Lynx/Voxel/Voxel.h"
#include "Lynx/Render/VertexArray.h"
#include "Lynx/Render/Buffer.h"
#include "Lynx/Detail/glm.h"

namespace Lynx {
	struct VertexData {
		glm::vec3 Pos;
		glm::vec3 Color;
		int ActiveSides;
	};

	class Chunk {
	public:
		Chunk();
		void Create(float x, float y, float z);

		void Update();
		void Render();

		Voxel::Type& GetVoxel(int x, int y, int z);

		static int IndexLinear(int x, int y, int z);
		static bool Inside(int x, int y, int z);
		static const int SIZE = 16;
	private:
		int ActiveSidesOfVoxel(int x, int y, int z);
		void CreateVoxelData();
		glm::vec3 m_Position{ 0.0f };
		std::vector<Voxel::Type> m_Voxels;
		std::vector<VertexData> m_VoxelData;
		VertexArray m_VA;
		std::shared_ptr<VertexBuffer> m_VB = nullptr;
	};
}