#include "Lynxpch.h"
#include "VoxelMachine.h"
#include "Lynx/Utility/BitMask.h"
#include "glad/glad.h"

namespace Lynx {
	constexpr int SIX_SIDES = 63;
	constexpr int NX_SIDE = BIT(0);
	constexpr int PX_SIDE = BIT(1);
	constexpr int NY_SIDE = BIT(2);
	constexpr int PY_SIDE = BIT(3);
	constexpr int NZ_SIDE = BIT(4);
	constexpr int PZ_SIDE = BIT(5);

	void VoxelMachine::CreateBox(glm::ivec3 size)
	{
		m_Size = size;
		int linearSize = size.x * size.y * size.z;
		m_Voxels.reserve(linearSize);
		m_VoxelVertices.reserve(linearSize);

		///////
		//Fill voxel array
		for (int x = 0; x < size.x; x++) {
			for (int y = 0; y < size.y; y++) {
				for (int z = 0; z < size.z; z++) {
					glm::vec3 color = { (float)x / size.x, (float)y / size.y, (float)z / size.z };
					m_Voxels.emplace_back(Voxel{ { x * 0.5f, y * 0.5f, z * 0.5f }, color, true, false, 0 });
				}
			}
		}
		///////
		//Fill voxel vertex array
		m_VBEndIndex = 0;
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
						int index = LinearFrom3D({ x, y, z });
						m_VoxelVertices.emplace_back(VoxelVertex{ m_Voxels[index].Pos, m_Voxels[index].Color, visibleSides });
						m_Voxels[index].VBIndex = m_VBEndIndex;
						m_Voxels[index].Visible = true;
						m_VBEndIndex++;
					}
				}
			}
		}
		//////////
		LX_CORE_TRACE("Voxel memory used: {0} bytes", m_VoxelVertices.size() * sizeof(VoxelVertex));
		m_TotalVoxels = m_VoxelVertices.size();
		m_VoxelVB = std::make_shared<VertexBuffer>(&m_VoxelVertices[0], linearSize * sizeof(VoxelVertex));
		m_VoxelVB->SetLayout({
			{ Lynx::ShaderDataType::Float3, "a_Position" },
			{ Lynx::ShaderDataType::Float3, "a_Color" },
			{ Lynx::ShaderDataType::Int, "a_EnabledSides" }
		});
		m_VoxelVA.AddVertexBuffer(m_VoxelVB);
	}

	int VoxelMachine::LinearFrom3D(const glm::ivec3& pos)
	{
		int a = m_Size.z * m_Size.y;
		int b = m_Size.z;
		return a * pos.x + b * pos.y + pos.z;
	}

	Voxel& VoxelMachine::GetVoxel(glm::ivec3 pos)
	{
		if (!InBounds(pos))
			LX_CORE_ASSERT(true, "Voxel is out of bounds");
		return m_Voxels[LinearFrom3D(pos)];
	}

	bool VoxelMachine::InBounds(glm::ivec3 pos)
	{
		bool upper = pos.x >= m_Size.x || pos.y >= m_Size.y || pos.z >= m_Size.z;
		bool lower = pos.x < 0.0f || pos.y < 0.0f || pos.z < 0.0f;
		if (upper || lower)
			return false;
		return true;
	}

	void VoxelMachine::Bind()
	{
		m_VoxelVA.Bind();
	}

	void VoxelMachine::Draw()
	{
		glDrawArrays(GL_POINTS, 0, (GLsizei)m_VBEndIndex);
	}

	void VoxelMachine::Delete(glm::ivec3 pos)
	{
		uint32_t index = LinearFrom3D(pos);
		//m_Voxels[index].Active = false;

		if (m_Voxels[index].Active) {
			m_Voxels[index].Active = false;
			uint32_t vbIndex = m_Voxels[index].VBIndex;
			//Swap vertex data
			m_VoxelVertices[vbIndex] = m_VoxelVertices[m_VBEndIndex - 1];
			//Swap VB index
			for (auto i = m_Voxels.begin(); i != m_Voxels.end(); ++i) {
				if (i->VBIndex == m_VBEndIndex - 1)
					i->VBIndex = vbIndex;
			}
			//decrement vboEndIndex
			m_VBEndIndex--;
			//write buffer data
			glBufferSubData(GL_ARRAY_BUFFER, vbIndex * sizeof(VoxelVertex), sizeof(VoxelVertex), &m_VoxelVertices[vbIndex]);
			
			int x = pos.x;
			int y = pos.y;
			int z = pos.z;
			int addedAdjacentVoxels = 0;
			int oldVBEndIndex = m_VBEndIndex;
			//std::vector<VoxelVertex> voxelVerticesToAdd;
			/*
			if (x > 0 && m_Voxels[LinearFrom3D({ x - 1, y, z })].Active) {
				int adjIndex = LinearFrom3D({ x - 1, y, z });
				m_VoxelVertices.at(adjIndex) = VoxelVertex{ m_Voxels[adjIndex].Pos, m_Voxels[adjIndex].Color, VisibleSides({ x - 1, y, z }) };
				m_Voxels[index].VBIndex = m_VBEndIndex;
				m_Voxels[index].Visible = true;
				m_VBEndIndex++;
				addedAdjacentVoxels++;
			}
			if (x < m_Size.x - 1 && m_Voxels[LinearFrom3D({ x + 1, y, z })].Active) {
				int adjIndex = LinearFrom3D({ x + 1, y, z });
				m_VoxelVertices.at(adjIndex) = VoxelVertex{ m_Voxels[adjIndex].Pos, m_Voxels[adjIndex].Color, VisibleSides({ x + 1, y, z }) };
				m_Voxels[index].VBIndex = m_VBEndIndex;
				m_Voxels[index].Visible = true;
				m_VBEndIndex++;
				addedAdjacentVoxels++;
			}
			if (y > 0 && m_Voxels[LinearFrom3D({ x, y - 1, z })].Active) {
				int adjIndex = LinearFrom3D({ x, y - 1, z });
				m_VoxelVertices.at(adjIndex) = VoxelVertex{ m_Voxels[adjIndex].Pos, m_Voxels[adjIndex].Color, VisibleSides({ x, y - 1, z }) };
				m_Voxels[index].VBIndex = m_VBEndIndex;
				m_Voxels[index].Visible = true;
				m_VBEndIndex++;
				addedAdjacentVoxels++;
			}
			if (y < m_Size.y - 1 && m_Voxels[LinearFrom3D({ x, y + 1, z })].Active) {
				int adjIndex = LinearFrom3D({ x, y + 1, z });
				m_VoxelVertices.at(adjIndex) = VoxelVertex{ m_Voxels[adjIndex].Pos, m_Voxels[adjIndex].Color, VisibleSides({ x, y + 1, z }) };
				m_Voxels[index].VBIndex = m_VBEndIndex;
				m_Voxels[index].Visible = true;
				m_VBEndIndex++;
				addedAdjacentVoxels++;
			}
			if (z > 0 && m_Voxels[LinearFrom3D({ x, y, z - 1 })].Active) {
				int adjIndex = LinearFrom3D({ x, y, z - 1 });
				m_VoxelVertices.at(adjIndex) = VoxelVertex{ m_Voxels[adjIndex].Pos, m_Voxels[adjIndex].Color, VisibleSides({ x, y, z - 1 }) };
				m_Voxels[index].VBIndex = m_VBEndIndex;
				m_Voxels[index].Visible = true;
				m_VBEndIndex++;
				addedAdjacentVoxels++;
			}
			if (z < m_Size.z - 1 && m_Voxels[LinearFrom3D({ x, y, z + 1 })].Active) {
				int adjIndex = LinearFrom3D({ x, y, z + 1 });
				m_VoxelVertices.at(adjIndex) = VoxelVertex{ m_Voxels[adjIndex].Pos, m_Voxels[adjIndex].Color, VisibleSides({ x, y, z + 1 }) };
				m_Voxels[index].VBIndex = m_VBEndIndex;
				m_Voxels[index].Visible = true;
				m_VBEndIndex++;
				addedAdjacentVoxels++;
			}
			//for (int i = 0; i < voxelVerticesToAdd.size(); i++)
				//m_VoxelVertices.emplace_back(voxelVerticesToAdd.at(i));
			glBufferSubData(GL_ARRAY_BUFFER, oldVBEndIndex * sizeof(VoxelVertex), addedAdjacentVoxels * sizeof(VoxelVertex), &m_VoxelVertices[oldVBEndIndex]);
			*/
		}
	}
	int VoxelMachine::VisibleSides(glm::ivec3 pos)
	{
		int x = pos.x;
		int y = pos.y;
		int z = pos.z;
		int obscuredSides = 0;

		if (x > 0 && m_Voxels[LinearFrom3D({ x - 1, y, z })].Active) {
			obscuredSides += BIT(0);
		}
		if (x < m_Size.x - 1 && m_Voxels[LinearFrom3D({ x + 1, y, z })].Active) {
			obscuredSides += BIT(1);
		}
		if (y > 0 && m_Voxels[LinearFrom3D({ x, y - 1, z })].Active) {
			obscuredSides += BIT(2);
		}
		if (y < m_Size.y - 1 && m_Voxels[LinearFrom3D({ x, y + 1, z })].Active) {
			obscuredSides += BIT(3);
		}
		if (z > 0 && m_Voxels[LinearFrom3D({ x, y, z - 1 })].Active) {
			obscuredSides += BIT(4);
		}
		if (z < m_Size.z - 1 && m_Voxels[LinearFrom3D({ x, y, z + 1 })].Active) {
			obscuredSides += BIT(5);
		}
		return SIX_SIDES - obscuredSides;
	}
}