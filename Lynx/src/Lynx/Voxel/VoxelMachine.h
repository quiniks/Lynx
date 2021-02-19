#pragma once
#include "Lynx/Detail/glm.h"
#include "Lynx/Render/VertexArray.h"
#include "Lynx/Render/Buffer.h"

namespace Lynx {
	struct Voxel {
		glm::vec3 Pos;
		glm::vec3 Color;
		bool Active;
	};

	struct VoxelVertex {
		glm::vec3 Pos;
		glm::vec3 Color;
		int Sides;
	};

	class VoxelMachine {
	public:
		VoxelMachine() = default;
		void CreateBox(glm::ivec3 size);
		void UpdateBox();

		int LinearFrom3D(const glm::ivec3& pos);
		Voxel& GetVoxel(glm::ivec3 pos);
		bool InBounds(glm::ivec3 pos);

		void Bind();
		void Draw();

		void Delete(glm::ivec3 pos);

		int VisibleSides(glm::ivec3 pos);
	private:
		glm::ivec3 m_Size = { 0, 0, 0 };
		std::vector<Voxel> m_Voxels;

		VertexArray m_VoxelVA;
		std::shared_ptr<VertexBuffer> m_VoxelVB = nullptr;
		std::vector<VoxelVertex> m_VoxelVertices;
		uint32_t m_VBEndIndex = 0;
		size_t m_TotalVoxels = 0;
	};
}