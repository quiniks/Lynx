#include "Lynxpch.h"
#include "VoxelMachine.h"
#include "Lynx/Utility/BitMask.h"
#include "glad/glad.h"

void Lynx::VoxelMachine::CreateBox(glm::ivec3 size)
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
				if (z % 2)
					m_Voxels.emplace_back(Voxel{ { x, y, z }, color, true, true });
				else
					m_Voxels.emplace_back(Voxel{ { x, y, z }, color, false, true });
			}
		}
	}
	///////
	//Validate surface voxels and fill vertex array
	std::vector<VoxelVertex> voxelVertices;
	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			for (int z = 0; z < size.z; z++) {
				int adjacentVoxels = 0;
				int surfaceSides = 0;
				if (x > 0) {
					if (m_Voxels[LinearFrom3D({ x - 1, y, z })].Active)
						adjacentVoxels += BIT(0);
					else
						surfaceSides += BIT(0);
				}
				else {
					surfaceSides += BIT(0);
				}
				if (x < size.x - 1) {
					if (m_Voxels[LinearFrom3D({ x + 1, y, z })].Active)
						adjacentVoxels += BIT(1);
					else
						surfaceSides += BIT(1);
				}
				else {
					surfaceSides += BIT(1);
				}
				if (y > 0) {
					if (m_Voxels[LinearFrom3D({ x, y - 1, z })].Active)
						adjacentVoxels += BIT(2);
					else
						surfaceSides += BIT(2);
				}
				else {
					surfaceSides += BIT(2);
				}
				if (y < size.y - 1) {
					if (m_Voxels[LinearFrom3D({ x, y + 1, z })].Active)
						adjacentVoxels += BIT(3);
					else
						surfaceSides += BIT(3);
				}
				else {
					surfaceSides += BIT(3);
				}
				if (z > 0) {
					if (m_Voxels[LinearFrom3D({ x, y, z - 1 })].Active)
						adjacentVoxels += BIT(4);
					else
						surfaceSides += BIT(4);
				}
				else {
					surfaceSides += BIT(4);
				}
				if (z < size.z - 1) {
					if (m_Voxels[LinearFrom3D({ x, y, z + 1 })].Active)
						adjacentVoxels += BIT(5);
					else
						surfaceSides += BIT(5);
				}
				else {
					surfaceSides += BIT(5);
				}
				if (adjacentVoxels == 63) {
					m_Voxels[LinearFrom3D({ x, y, z })].Surface = false;
				}
				else {
					if (m_Voxels[LinearFrom3D({ x, y, z })].Active)
						voxelVertices.emplace_back(VoxelVertex{ m_Voxels[LinearFrom3D({ x, y, z })].Pos, m_Voxels[LinearFrom3D({ x, y, z })].Color, surfaceSides });
				}
			}
		}
	}
	//////////
	LX_CORE_INFO("Memory used: {0}", voxelVertices.size() * sizeof(VoxelVertex));
	m_TotalVoxels = voxelVertices.size();
	m_VoxelVB = std::make_shared<VertexBuffer>(&voxelVertices[0], voxelVertices.size() * sizeof(VoxelVertex));
	m_VoxelVB->SetLayout({
		{ Lynx::ShaderDataType::Float3, "a_Position" },
		{ Lynx::ShaderDataType::Float3, "a_Color" },
		{ Lynx::ShaderDataType::Int, "a_EnabledSides" }
	});
	m_VoxelVA.AddVertexBuffer(m_VoxelVB);
}

int Lynx::VoxelMachine::LinearFrom3D(const glm::ivec3& index)
{
	int a = m_Size.z * m_Size.y;
	int b = m_Size.z;
	return a * index.x + b * index.y + index.z;
}

void Lynx::VoxelMachine::Bind()
{
	m_VoxelVA.Bind();
}

void Lynx::VoxelMachine::Draw()
{
	glDrawArrays(GL_POINTS, 0, m_TotalVoxels);
}