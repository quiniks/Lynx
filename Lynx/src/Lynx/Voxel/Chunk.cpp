#include "Lynxpch.h"
#include "Chunk.h"
#include "World.h"
#include "Lynx/Utility/BitMask.h"
#include "glad/glad.h"

namespace Lynx {
	Chunk::Chunk(World& world, int x, int y, int z) : m_World(world)
	{
		m_ChunkPosition = { x, y, z };
		//m_World = world;

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

		CreateVoxelData();
		//////////
		m_VA = VertexArray::Create();
		m_VB = VertexBuffer::Create(&m_VoxelData[0], m_VoxelData.size() * sizeof(VertexData));
		m_VB->SetLayout({
			{ Lynx::ShaderDataType::Float3, "a_Position" },
			{ Lynx::ShaderDataType::Float3, "a_Color" },
			{ Lynx::ShaderDataType::Int, "a_ActiveSides" }
			});
		m_VA->AddVertexBuffer(m_VB);
	}

	void Chunk::Update()
	{
		m_VoxelData.clear();
		CreateVoxelData();
		m_VB->SetData(&m_VoxelData[0], m_VoxelData.size() * sizeof(VertexData));
	}

	void Chunk::Render()
	{
		m_VA->Bind();
		glDrawArrays(GL_POINTS, 0, (GLsizei)m_VoxelData.size());
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

	void Chunk::AllActiveSides(std::vector<int>& sides)
	{
		sides.resize((uint32_t)pow(SIZE, 3), Voxel::NO_SIDES);


		for (int x = 0; x < SIZE; x++) {
			for (int y = 0; y < SIZE; y++) {
				for (int z = 0; z < SIZE; z++) {

					int& SidesThis = sides.at(IndexLinear(x, y, z));
					//X
					if (x > 0 && m_Voxels.at(IndexLinear(x - 1, y, z)) == Voxel::Type::Empty) {
						SidesThis += Voxel::NX_SIDE;
						sides.at(IndexLinear(x - 1, y, z)) += Voxel::PX_SIDE;
					}
					else if (x == 0)
						SidesThis += AdjacentChunkCheckN(m_ChunkPosition.x, { -1, 0, 0 }, { x, y, z }, Voxel::NX_SIDE);// Voxel::NX_SIDE;
					else if (x == SIZE - 1)
						SidesThis += AdjacentChunkCheckP(m_ChunkPosition.x, World::SIZE.x, { 1, 0, 0 }, { x, y, z }, Voxel::PX_SIDE);// Voxel::PX_SIDE;
					//Y
					if (y > 0 && m_Voxels.at(IndexLinear(x, y - 1, z)) == Voxel::Type::Empty) {
						SidesThis += Voxel::NY_SIDE;
						sides.at(IndexLinear(x, y - 1, z)) += Voxel::PY_SIDE;
					}
					else if (y == 0)
						SidesThis += AdjacentChunkCheckN(m_ChunkPosition.y, { 0, -1, 0 }, { x, y, z }, Voxel::NY_SIDE);// Voxel::NY_SIDE;
					else if (y == SIZE - 1)
						SidesThis += AdjacentChunkCheckP(m_ChunkPosition.y, World::SIZE.y, { 0, 1, 0 }, { x, y, z }, Voxel::PY_SIDE);// Voxel::PY_SIDE;
					//Z
					if (z > 0 && m_Voxels.at(IndexLinear(x, y, z - 1)) == Voxel::Type::Empty) {
						SidesThis += Voxel::NZ_SIDE;
						sides.at(IndexLinear(x, y, z - 1)) += Voxel::PZ_SIDE;
					}
					else if (z == 0)
						SidesThis += AdjacentChunkCheckN(m_ChunkPosition.z, { 0, 0, -1 }, { x, y, z }, Voxel::NZ_SIDE);// Voxel::NZ_SIDE;
					else if (z == SIZE - 1)
						SidesThis += AdjacentChunkCheckP(m_ChunkPosition.z, World::SIZE.z, { 0, 0, 1 }, { x, y, z }, Voxel::PZ_SIDE);// Voxel::PZ_SIDE;
				}
			}
		}
	}

	int Chunk::AdjacentChunkCheckN(int chunkAxisPos, const glm::ivec3& offset, const glm::ivec3& voxelPos, int side)
	{
		if (chunkAxisPos > 0) {
			Chunk& AdjChunk = m_World.GetChunk(m_ChunkPosition.x + offset.x, m_ChunkPosition.y + offset.y, m_ChunkPosition.z + offset.z);
			bool isAdjChunkVoxelEmpty = false;
			if (offset.x != 0)
				isAdjChunkVoxelEmpty = AdjChunk.GetVoxel(SIZE - 1, voxelPos.y, voxelPos.z) == Voxel::Type::Empty;
			else if (offset.y != 0)
				isAdjChunkVoxelEmpty = AdjChunk.GetVoxel(voxelPos.x, SIZE - 1, voxelPos.z) == Voxel::Type::Empty;
			else
				isAdjChunkVoxelEmpty = AdjChunk.GetVoxel(voxelPos.x, voxelPos.y, SIZE - 1) == Voxel::Type::Empty;

			if (isAdjChunkVoxelEmpty)
				return side;
		}
		else {
			return side;
		}
		return Voxel::NO_SIDES;
	}

	int Chunk::AdjacentChunkCheckP(int chunkAxisPos, int worldAxisSize, const glm::ivec3& offset, const glm::ivec3& voxelPos, int side)
	{
		if (chunkAxisPos < worldAxisSize - 1) {
			Chunk& AdjChunk = m_World.GetChunk(m_ChunkPosition.x + offset.x, m_ChunkPosition.y + offset.y, m_ChunkPosition.z + offset.z);
			bool isAdjChunkVoxelEmpty = false;
			if (offset.x != 0)
				isAdjChunkVoxelEmpty = AdjChunk.GetVoxel(0, voxelPos.y, voxelPos.z) == Voxel::Type::Empty;
			else if (offset.y != 0)
				isAdjChunkVoxelEmpty = AdjChunk.GetVoxel(voxelPos.x, 0, voxelPos.z) == Voxel::Type::Empty;
			else
				isAdjChunkVoxelEmpty = AdjChunk.GetVoxel(voxelPos.x, voxelPos.y, 0) == Voxel::Type::Empty;

			if (isAdjChunkVoxelEmpty)
				return side;
		}
		else {
			return side;
		}
		return Voxel::NO_SIDES;
	}

	void Chunk::CreateVoxelData() {
		std::vector<int> sides;
		AllActiveSides(sides);

		glm::vec3 color = (glm::vec3)m_ChunkPosition / (glm::vec3)m_World.SIZE + glm::vec3{ 0.8f, 0.2f, 0.3f };

		m_VoxelData.reserve((uint32_t)pow(SIZE, 3));
		for (int x = 0; x < SIZE; x++) {
			for (int y = 0; y < SIZE; y++) {
				for (int z = 0; z < SIZE; z++) {
					int index = IndexLinear(x, y, z);
					if (m_Voxels.at(index) != Voxel::Type::Empty) {
						m_VoxelData.emplace_back(VertexData{ 
							{ x * Voxel::SIZE + m_Position.x, y * Voxel::SIZE + m_Position.y, z * Voxel::SIZE + m_Position.z }, 
							color,
							ActiveSidesOfVoxel(x, y, z)/*sides.at(IndexLinear(x, y, z))*/
						});
					}
				}
			}
		}
	}
}