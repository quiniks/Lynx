#include "Lynxpch.h"
#include "VoxelMachine.h"
#include "Lynx/Utility/BitMask.h"
#include "glad/glad.h"

namespace Lynx {
	constexpr int SIX_SIDES = 63;
	constexpr int NO_SIDES = 63;

	void VoxelMachine::CreateBox(glm::ivec3 size)
	{
		m_Size = size;
		int linearSize = size.x * size.y * size.z;
		m_Voxels.reserve(linearSize);
		//m_VoxelVertices.reserve(linearSize);

		///////
		//Fill voxel array
		for (int x = 0; x < size.x; x++) {
			for (int y = 0; y < size.y; y++) {
				for (int z = 0; z < size.z; z++) {
					glm::vec3 color = { (float)x / size.x, (float)y / size.y, (float)z / size.z };
					m_Voxels.emplace_back(Voxel{ { x * 0.5f, y * 0.5f, z * 0.5f }, color, true});
				}
			}
		}
		///////
		//Fill voxel vertex array
		m_VBEndIndex = 0;
		for (int x = 0; x < size.x; x++) {
			for (int y = 0; y < size.y; y++) {
				for (int z = 0; z < size.z; z++) {
					int index = LinearFrom3D({ x, y, z });
					if (m_Voxels[index].Active)
						m_VoxelVertices.emplace_back(VoxelVertex{ m_Voxels[index].Pos, m_Voxels[index].Color, VisibleSides({ x, y, z }) });
				}
			}
		}
		//////////
		LX_CORE_TRACE("Voxel data:");
		LX_CORE_TRACE(" Total: {0} voxels at {1} bytes", m_VoxelVertices.size(), m_VoxelVertices.size() * sizeof(VoxelVertex));
		LX_CORE_TRACE(" Voxel: {0} bytes", sizeof(VoxelVertex));
		m_TotalVoxels = m_VoxelVertices.size();
		m_VoxelVB = std::make_shared<VertexBuffer>(&m_VoxelVertices[0], linearSize * sizeof(VoxelVertex));
		m_VoxelVB->SetLayout({
			{ Lynx::ShaderDataType::Float3, "a_Position" },
			{ Lynx::ShaderDataType::Float3, "a_Color" },
			{ Lynx::ShaderDataType::Int, "a_EnabledSides" }
		});
		m_VoxelVA.AddVertexBuffer(m_VoxelVB);
	}

	void VoxelMachine::UpdateBox()
	{
		m_VoxelVertices.clear();
		
		for (int x = 0; x < m_Size.x; x++) {
			for (int y = 0; y < m_Size.y; y++) {
				for (int z = 0; z < m_Size.z; z++) {
					int index = LinearFrom3D({ x, y, z });
					if (m_Voxels[index].Active == true) {
						m_VoxelVertices.emplace_back(VoxelVertex{ m_Voxels[index].Pos, m_Voxels[index].Color, VisibleSides({ x, y, z }) });
					}
				}
			}
		}

		m_VoxelVB->SetData(&m_VoxelVertices[0], m_VoxelVertices.size() * sizeof(VoxelVertex));
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
		glDrawArrays(GL_POINTS, 0, (GLsizei)m_VoxelVertices.size());
	}

	void VoxelMachine::Delete(glm::ivec3 pos)
	{

		/*
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
			std::vector<VoxelVertex> voxelVerticesToAdd;
			
			if (x > 0 && m_Voxels[LinearFrom3D({ x - 1, y, z })].Active) {
				int adjIndex = LinearFrom3D({ x - 1, y, z });
				voxelVerticesToAdd.emplace_back(VoxelVertex{ m_Voxels[adjIndex].Pos, m_Voxels[adjIndex].Color, VisibleSides({ x - 1, y, z }) });
				m_Voxels[index].VBIndex = m_VBEndIndex;
				m_Voxels[index].Visible = true;
				m_VBEndIndex++;
				addedAdjacentVoxels++;
			}
			if (x < m_Size.x - 1 && m_Voxels[LinearFrom3D({ x + 1, y, z })].Active) {
				int adjIndex = LinearFrom3D({ x + 1, y, z });
				voxelVerticesToAdd.emplace_back(VoxelVertex{ m_Voxels[adjIndex].Pos, m_Voxels[adjIndex].Color, VisibleSides({ x + 1, y, z }) });
				m_Voxels[index].VBIndex = m_VBEndIndex;
				m_Voxels[index].Visible = true;
				m_VBEndIndex++;
				addedAdjacentVoxels++;
			}
			if (y > 0 && m_Voxels[LinearFrom3D({ x, y - 1, z })].Active) {
				int adjIndex = LinearFrom3D({ x, y - 1, z });
				voxelVerticesToAdd.emplace_back(VoxelVertex{ m_Voxels[adjIndex].Pos, m_Voxels[adjIndex].Color, VisibleSides({ x, y - 1, z }) });
				m_Voxels[index].VBIndex = m_VBEndIndex;
				m_Voxels[index].Visible = true;
				m_VBEndIndex++;
				addedAdjacentVoxels++;
			}
			if (y < m_Size.y - 1 && m_Voxels[LinearFrom3D({ x, y + 1, z })].Active) {
				int adjIndex = LinearFrom3D({ x, y + 1, z });
				voxelVerticesToAdd.emplace_back(VoxelVertex{ m_Voxels[adjIndex].Pos, m_Voxels[adjIndex].Color, VisibleSides({ x, y + 1, z }) });
				m_Voxels[index].VBIndex = m_VBEndIndex;
				m_Voxels[index].Visible = true;
				m_VBEndIndex++;
				addedAdjacentVoxels++;
			}
			if (z > 0 && m_Voxels[LinearFrom3D({ x, y, z - 1 })].Active) {
				int adjIndex = LinearFrom3D({ x, y, z - 1 });
				voxelVerticesToAdd.emplace_back(VoxelVertex{ m_Voxels[adjIndex].Pos, m_Voxels[adjIndex].Color, VisibleSides({ x, y, z - 1 }) });
				m_Voxels[index].VBIndex = m_VBEndIndex;
				m_Voxels[index].Visible = true;
				m_VBEndIndex++;
				addedAdjacentVoxels++;
			}
			if (z < m_Size.z - 1 && m_Voxels[LinearFrom3D({ x, y, z + 1 })].Active) {
				int adjIndex = LinearFrom3D({ x, y, z + 1 });
				voxelVerticesToAdd.emplace_back(VoxelVertex{ m_Voxels[adjIndex].Pos, m_Voxels[adjIndex].Color, VisibleSides({ x, y, z + 1 }) });
				m_Voxels[index].VBIndex = m_VBEndIndex;
				m_Voxels[index].Visible = true;
				m_VBEndIndex++;
				addedAdjacentVoxels++;
			}
			for (int i = 0; i < voxelVerticesToAdd.size(); i++)
				m_VoxelVertices.at(oldVBEndIndex + i) = voxelVerticesToAdd.at(i);
			glBufferSubData(GL_ARRAY_BUFFER, oldVBEndIndex * sizeof(VoxelVertex), addedAdjacentVoxels * sizeof(VoxelVertex), &voxelVerticesToAdd[0]);
			
		}
		*/
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