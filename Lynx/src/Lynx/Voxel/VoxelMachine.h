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
		int LinearFrom3D(const glm::ivec3& index);
		void Bind();
		void Draw();
	private:
		glm::ivec3 m_Size = { 0, 0, 0 };
		std::vector<Voxel> m_Voxels;

		VertexArray m_VoxelVA;
		std::shared_ptr<VertexBuffer> m_VoxelVB = nullptr;
		size_t m_TotalVoxels = 0;
	};
}