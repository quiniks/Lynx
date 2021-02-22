#include "Lynxpch.h"
#include "Chunk.h"
#include "Lynx/Utility/BitMask.h"
#include "glad/glad.h"

namespace Lynx {
	Chunk::Chunk()
	{
		for (int x = 0; x < SIZE; x++) {
			for (int y = 0; y < SIZE; y++) {
				for (int z = 0; z < SIZE; z++) {
					m_Voxels.emplace_back(Voxel::Type::Solid);
				}
			}
		}
	}
	void Chunk::Create(float x, float y, float z)
	{
		m_Position = { x, y, z };

		CreateVoxelData();
		//////////
		m_VB = std::make_shared<VertexBuffer>(&m_VoxelData[0], m_VoxelData.size() * sizeof(VertexData));
		m_VB->SetLayout({
			{ Lynx::ShaderDataType::Float3, "a_Position" },
			{ Lynx::ShaderDataType::Float3, "a_Color" },
			{ Lynx::ShaderDataType::Int, "a_ActiveSides" }
			});
		m_VA.AddVertexBuffer(m_VB);
	}

	void Chunk::Update()
	{
		m_VoxelData.clear();
		CreateVoxelData();
		m_VB->SetData(&m_VoxelData[0], m_VoxelData.size() * sizeof(VertexData));
	}

	void Chunk::Render()
	{
		m_VA.Bind();
		glDrawArrays(GL_POINTS, 0, (GLsizei)m_VoxelData.size());
	}

	Voxel::Type& Chunk::GetVoxel(int x, int y, int z) {
		if (!Inside(x, y, z))
			LX_CORE_ASSERT(true, "Voxel is not inside chunk");
		return m_Voxels.at(IndexLinear(x, y, z));
	}

	int Chunk::IndexLinear(int x, int y, int z)
	{
		int a = SIZE * SIZE;	//Z * Y
		int b = SIZE;			//Z
		return a * x + b * y + z;
	}

	bool Chunk::Inside(int x, int y, int z)
	{
		bool upper = x >= SIZE || y >= SIZE || z >= SIZE;
		bool lower = x < 0.0f || y < 0.0f || z < 0.0f;
		if (upper || lower)
			return false;
		return true;
	}

	int Chunk::ActiveSidesOfVoxel(int x, int y, int z)
	{
		int obscuredSides = Voxel::NO_SIDES;

		if (x > 0 && m_Voxels.at(IndexLinear(x - 1, y, z)) != Voxel::Type::Empty) {
			obscuredSides += BIT(0);
		}
		if (x < SIZE - 1 && m_Voxels.at(IndexLinear(x + 1, y, z)) != Voxel::Type::Empty) {
			obscuredSides += BIT(1);
		}
		if (y > 0 && m_Voxels.at(IndexLinear(x, y - 1, z)) != Voxel::Type::Empty) {
			obscuredSides += BIT(2);
		}
		if (y < SIZE - 1 && m_Voxels.at(IndexLinear(x, y + 1, z)) != Voxel::Type::Empty) {
			obscuredSides += BIT(3);
		}
		if (z > 0 && m_Voxels.at(IndexLinear(x, y, z - 1)) != Voxel::Type::Empty) {
			obscuredSides += BIT(4);
		}
		if (z < SIZE - 1 && m_Voxels.at(IndexLinear(x, y, z + 1)) != Voxel::Type::Empty) {
			obscuredSides += BIT(5);
		}
		return Voxel::SIX_SIDES - obscuredSides;
	}

	void Chunk::CreateVoxelData() {
		for (int x = 0; x < SIZE; x++) {
			for (int y = 0; y < SIZE; y++) {
				for (int z = 0; z < SIZE; z++) {
					int index = IndexLinear(x, y, z);
					if (m_Voxels.at(index) != Voxel::Type::Empty) {
						m_VoxelData.emplace_back(VertexData{ 
							{ x * Voxel::SIZE + m_Position.x, y * Voxel::SIZE + m_Position.y, z * Voxel::SIZE + m_Position.z }, 
							{ 0.8f, 0.2f, 0.3f },
							ActiveSidesOfVoxel(x, y, z) 
						});
					}
				}
			}
		}
	}
}