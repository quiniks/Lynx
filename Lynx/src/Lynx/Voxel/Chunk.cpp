#include "Lynxpch.h"
#include "Chunk.h"
#include "World.h"
#include "Lynx/Utility/BitMask.h"
#include "glad/glad.h"

namespace Lynx {
	Chunk::Chunk(World& world, int x, int y, int z) : m_World(world)
	{
		m_ChunkPosition = { x, y, z };

		m_Voxels.reserve((uint32_t)pow(SIZE, 3));
		for (int x = 0; x < SIZE; x++) {
			for (int y = 0; y < SIZE; y++) {
				for (int z = 0; z < SIZE; z++) {
					m_Voxels.emplace_back(Voxel::Type::Solid);
				}
			}
		}
	}

	void Chunk::CreateMesh(float x, float y, float z)
	{
		m_Position = { x, y, z };
		int maxVertices = 36 * (int)pow(SIZE, 3);
		//////////
		m_VA = VertexArray::Create();
		m_VB = VertexBuffer::Create(nullptr, maxVertices * sizeof(VertexData));
		m_VB->SetLayout({
			{ Lynx::ShaderDataType::Float3, "a_Position" },
			{ Lynx::ShaderDataType::Float3, "a_Color" },
			{ Lynx::ShaderDataType::Float3, "a_Normal" }
			});
		m_VA->AddVertexBuffer(m_VB);

		CreateVoxelData();
		LX_INFO("init size: {0}", m_VertexData.size());
		m_VB->SetData(m_VertexData.data(), m_VertexData.size() * sizeof(VertexData));
	}

	void Chunk::Update()
	{
		m_VertexData.clear();
		CreateVoxelData();
		//m_VB->Test(m_VertexData.data(), m_VertexData.size() * sizeof(VertexData));
		m_VB->SetData(m_VertexData.data(), m_VertexData.size() * sizeof(VertexData));
	}

	void Chunk::Render()
	{
		m_VA->Bind();
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_VertexData.size());
	}

	Voxel::Type& Chunk::GetVoxel(int x, int y, int z) {
		if (!Inside(x, y, z))
			LX_CORE_ASSERT(false, "Voxel is not inside chunk");
		return m_Voxels.at(IndexLinear(x, y, z));
	}

	int Chunk::IndexLinear(int x, int y, int z)
	{
		int a = SIZE * SIZE;	//Z * Y
		int b = SIZE;			//Z
		return a * x + b * y + z;
	}

	int Chunk::VIndexLinear(int x, int y, int z)
	{
		int a = (SIZE + 1) * (SIZE + 1);	//Z * Y
		int b = (SIZE + 1);			//Z
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
			obscuredSides += Voxel::NX_SIDE;
		}
		if (x < SIZE - 1 && m_Voxels.at(IndexLinear(x + 1, y, z)) != Voxel::Type::Empty) {
			obscuredSides += Voxel::PX_SIDE;
		}
		if (y > 0 && m_Voxels.at(IndexLinear(x, y - 1, z)) != Voxel::Type::Empty) {
			obscuredSides += Voxel::NY_SIDE;
		}
		if (y < SIZE - 1 && m_Voxels.at(IndexLinear(x, y + 1, z)) != Voxel::Type::Empty) {
			obscuredSides += Voxel::PY_SIDE;
		}
		if (z > 0 && m_Voxels.at(IndexLinear(x, y, z - 1)) != Voxel::Type::Empty) {
			obscuredSides += Voxel::NZ_SIDE;
		}
		if (z < SIZE - 1 && m_Voxels.at(IndexLinear(x, y, z + 1)) != Voxel::Type::Empty) {
			obscuredSides += Voxel::PZ_SIDE;
		}
		return Voxel::SIX_SIDES - obscuredSides;
	}

	int Chunk::AddVertex(const glm::vec3& pos, const glm::vec3& color, const glm::vec3& normal)
	{
		glm::vec3 worldPos = (pos * Voxel::SIZE + m_Position);
		m_VertexData.emplace_back(VertexData{ worldPos, color, normal});
		//LX_INFO("worldPos: {0}, {1}, {2}", worldPos.x, worldPos.y, worldPos.z);
		return 0;//(int)m_VertexData.size() - 1;
	}

	void Chunk::CreateVoxelData() {
		m_VertexData.reserve((uint32_t)(pow(SIZE, 3) * 8));

		glm::vec3 color = (glm::vec3)m_ChunkPosition / (glm::vec3)m_World.SIZE + glm::vec3{ 0.8f, 0.2f, 0.3f };

		for (int x = 0; x < SIZE; x++) {
			for (int y = 0; y < SIZE; y++) {
				for (int z = 0; z < SIZE; z++) {
					int voxelIndex = IndexLinear(x, y, z);
					if (m_Voxels.at(voxelIndex) != Voxel::Type::Empty) {
						
						if (x == 0 || (x > 0 && m_Voxels.at(IndexLinear(x - 1, y, z)) == Voxel::Type::Empty)) {
							AddVertex({ 0.0f + x , 0.0f + y , 1.0f + z  }, color, { -1.0f, 0.0f, 0.0f });
							AddVertex({ 0.0f + x , 1.0f + y , 0.0f + z  }, color, { -1.0f, 0.0f, 0.0f });
							AddVertex({ 0.0f + x , 0.0f + y , 0.0f + z  }, color, { -1.0f, 0.0f, 0.0f });
							AddVertex({ 0.0f + x , 0.0f + y , 1.0f + z  }, color, { -1.0f, 0.0f, 0.0f });
							AddVertex({ 0.0f + x , 1.0f + y , 1.0f + z  }, color, { -1.0f, 0.0f, 0.0f });
							AddVertex({ 0.0f + x , 1.0f + y , 0.0f + z  }, color, { -1.0f, 0.0f, 0.0f });
						}

						if (x == SIZE - 1 || (x < SIZE - 1 && m_Voxels.at(IndexLinear(x + 1, y, z)) == Voxel::Type::Empty)) {
							AddVertex({ 1.0f + x, 0.0f + y, 0.0f + z }, color, { 1.0f, 0.0f, 0.0f });
							AddVertex({ 1.0f + x, 1.0f + y, 1.0f + z }, color, { 1.0f, 0.0f, 0.0f });
							AddVertex({ 1.0f + x, 0.0f + y, 1.0f + z }, color, { 1.0f, 0.0f, 0.0f });
							AddVertex({ 1.0f + x, 0.0f + y, 0.0f + z }, color, { 1.0f, 0.0f, 0.0f });
							AddVertex({ 1.0f + x, 1.0f + y, 0.0f + z }, color, { 1.0f, 0.0f, 0.0f });
							AddVertex({ 1.0f + x, 1.0f + y, 1.0f + z }, color, { 1.0f, 0.0f, 0.0f });
						}

						if (y == 0 || (y > 0 && m_Voxels.at(IndexLinear(x, y - 1, z)) == Voxel::Type::Empty)) {
							AddVertex({ 0.0f + x, 0.0f + y, 0.0f + z }, color, { 0.0f, -1.0f, 0.0f });
							AddVertex({ 1.0f + x, 0.0f + y, 1.0f + z }, color, { 0.0f, -1.0f, 0.0f });
							AddVertex({ 0.0f + x, 0.0f + y, 1.0f + z }, color, { 0.0f, -1.0f, 0.0f });
							AddVertex({ 0.0f + x, 0.0f + y, 0.0f + z }, color, { 0.0f, -1.0f, 0.0f });
							AddVertex({ 1.0f + x, 0.0f + y, 0.0f + z }, color, { 0.0f, -1.0f, 0.0f });
							AddVertex({ 1.0f + x, 0.0f + y, 1.0f + z }, color, { 0.0f, -1.0f, 0.0f });
						}

						if (z == SIZE - 1 || (z < SIZE - 1 && m_Voxels.at(IndexLinear(x, y, z + 1)) == Voxel::Type::Empty)) {
							AddVertex({ 1.0f + x, 0.0f + y, 1.0f + z }, color, { 0.0f, 0.0f, 1.0f });
							AddVertex({ 0.0f + x, 1.0f + y, 1.0f + z }, color, { 0.0f, 0.0f, 1.0f });
							AddVertex({ 0.0f + x, 0.0f + y, 1.0f + z }, color, { 0.0f, 0.0f, 1.0f });
							AddVertex({ 1.0f + x, 0.0f + y, 1.0f + z }, color, { 0.0f, 0.0f, 1.0f });
							AddVertex({ 1.0f + x, 1.0f + y, 1.0f + z }, color, { 0.0f, 0.0f, 1.0f });
							AddVertex({ 0.0f + x, 1.0f + y, 1.0f + z }, color, { 0.0f, 0.0f, 1.0f });
						}
						
						if (y == SIZE - 1 || (y < SIZE - 1 && m_Voxels.at(IndexLinear(x, y + 1, z)) == Voxel::Type::Empty)) {
							AddVertex({ 1.0f + x , 1.0f + y , 0.0f + z  }, color, { 0.0f, 1.0f, 0.0f });
							AddVertex({ 0.0f + x , 1.0f + y , 1.0f + z  }, color, { 0.0f, 1.0f, 0.0f });
							AddVertex({ 1.0f + x , 1.0f + y , 1.0f + z  }, color, { 0.0f, 1.0f, 0.0f });
							AddVertex({ 1.0f + x , 1.0f + y , 0.0f + z  }, color, { 0.0f, 1.0f, 0.0f });
							AddVertex({ 0.0f + x , 1.0f + y , 0.0f + z  }, color, { 0.0f, 1.0f, 0.0f });
							AddVertex({ 0.0f + x , 1.0f + y , 1.0f + z  }, color, { 0.0f, 1.0f, 0.0f });
						}
						
						if (z == 0 || (z > 0 && m_Voxels.at(IndexLinear(x, y, z - 1)) == Voxel::Type::Empty)) {
							AddVertex({ 0.0f + x, 0.0f + y, 0.0f + z }, color, { 0.0f, 0.0f, -1.0f });
							AddVertex({ 1.0f + x, 1.0f + y, 0.0f + z }, color, { 0.0f, 0.0f, -1.0f });
							AddVertex({ 1.0f + x, 0.0f + y, 0.0f + z }, color, { 0.0f, 0.0f, -1.0f });
							AddVertex({ 0.0f + x, 0.0f + y, 0.0f + z }, color, { 0.0f, 0.0f, -1.0f });
							AddVertex({ 0.0f + x, 1.0f + y, 0.0f + z }, color, { 0.0f, 0.0f, -1.0f });
							AddVertex({ 1.0f + x, 1.0f + y, 0.0f + z }, color, { 0.0f, 0.0f, -1.0f });
						}
					}
				}
			}
		}

	}
}