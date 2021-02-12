#include "Lynxpch.h"
#include "VoxelMachine.h"
#include "Lynx/Utility/BitMask.h"
#include "glad/glad.h"

namespace Lynx {
	constexpr int SIX_SIDES = 63;

	void VoxelMachine::CreateBox(glm::ivec3 size)
	{
		m_Size = size;
		int linearSize = size.x * size.y * size.z;
		m_Voxels.reserve(linearSize);

		///////
		//Fill voxel array
		for (int x = 0; x < size.x; x++) {
			for (int y = 0; y < size.y; y++) {
				for (int z = 0; z < size.z; z++) {
					glm::vec3 color = { (float)x / size.x, (float)y / size.y, (float)z / size.z };
					m_Voxels.emplace_back(Voxel{ { x * 0.5f, y * 0.5f, z * 0.5f }, color, true });
				}
			}
		}
		///////
		//Fill voxel vertex array
		std::vector<VoxelVertex> voxelVertices;
		for (int x = 0; x < size.x; x++) {
			for (int y = 0; y < size.y; y++) {
				for (int z = 0; z < size.z; z++) {
					//Skip loop if this voxel is inactive
					if (!m_Voxels[LinearFrom3D({ x, y, z })].Active)
						continue;
					//Determine obscured sides
					int obscuredSides = 0;
					if (x > 0 && m_Voxels[LinearFrom3D({ x - 1, y, z })].Active) {
						obscuredSides += BIT(0);
					}
					if (x < size.x - 1 && m_Voxels[LinearFrom3D({ x + 1, y, z })].Active) {
						obscuredSides += BIT(1);
					}
					if (y > 0 && m_Voxels[LinearFrom3D({ x, y - 1, z })].Active) {
						obscuredSides += BIT(2);
					}
					if (y < size.y - 1 && m_Voxels[LinearFrom3D({ x, y + 1, z })].Active) {
						obscuredSides += BIT(3);
					}
					if (z > 0 && m_Voxels[LinearFrom3D({ x, y, z - 1 })].Active) {
						obscuredSides += BIT(4);
					}
					if (z < size.z - 1 && m_Voxels[LinearFrom3D({ x, y, z + 1 })].Active) {
						obscuredSides += BIT(5);
					}
					//Add voxel vertex to array if it is visible
					if (obscuredSides != SIX_SIDES) {
						int visibleSides = SIX_SIDES - obscuredSides;
						voxelVertices.emplace_back(VoxelVertex{ m_Voxels[LinearFrom3D({ x, y, z })].Pos, m_Voxels[LinearFrom3D({ x, y, z })].Color, visibleSides });
					}
				}
			}
		}
		//////////
		LX_CORE_TRACE("Voxel memory used: {0} bytes", voxelVertices.size() * sizeof(VoxelVertex));
		m_TotalVoxels = voxelVertices.size();
		m_VoxelVB = std::make_shared<VertexBuffer>(&voxelVertices[0], linearSize * sizeof(VoxelVertex));
		m_VoxelVB->SetLayout({
			{ Lynx::ShaderDataType::Float3, "a_Position" },
			{ Lynx::ShaderDataType::Float3, "a_Color" },
			{ Lynx::ShaderDataType::Int, "a_EnabledSides" }
			});
		m_VoxelVA.AddVertexBuffer(m_VoxelVB);
	}

	int VoxelMachine::LinearFrom3D(const glm::ivec3& index)
	{
		int a = m_Size.z * m_Size.y;
		int b = m_Size.z;
		return a * index.x + b * index.y + index.z;
	}

	void VoxelMachine::Bind()
	{
		m_VoxelVA.Bind();
	}

	void VoxelMachine::Draw()
	{
		glDrawArrays(GL_POINTS, 0, (GLsizei)m_TotalVoxels);
	}
}