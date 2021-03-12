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
		m_Voxels.shrink_to_fit();
	}

	void Chunk::CreateMesh(float x, float y, float z)
	{
		m_Position = { x, y, z };

		m_VA = VertexArray::Create();

		CreateVoxelData();

		m_VB = VertexBuffer::Create(m_VertexData.data(), m_VertexData.size() * sizeof(VertexData));
		m_VB->SetLayout({
			{ Lynx::ShaderDataType::Float3, "a_Position" },
			{ Lynx::ShaderDataType::Float3, "a_Color" },
			{ Lynx::ShaderDataType::Int, "a_Side" },
			{ Lynx::ShaderDataType::Int, "a_AO" }
			});
		m_VA->AddVertexBuffer(m_VB);
	}

	void Chunk::Update()
	{
		CreateVoxelData();
		m_VB->SetData(m_VertexData.data(), m_VertexData.size() * sizeof(VertexData));
	}

	void Chunk::Render()
	{
		m_VA->Bind();
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_VertexData.size());
	}

	Voxel::Type Chunk::GetVoxel(int x, int y, int z) {
		glm::ivec3 voxelWorldPos = { m_ChunkPosition.x * SIZE + x, m_ChunkPosition.y * SIZE + y, m_ChunkPosition.z * SIZE + z };
		glm::ivec3 chunkPos = glm::floor((glm::vec3)voxelWorldPos / (float)Chunk::SIZE);
		bool validChunk = m_World.Inside(chunkPos.x, chunkPos.y, chunkPos.z);
		if (validChunk) {
			int vcpX = voxelWorldPos.x % Chunk::SIZE;
			int vcpY = voxelWorldPos.y % Chunk::SIZE;
			int vcpZ = voxelWorldPos.z % Chunk::SIZE;
			Voxel::Type type = m_World.GetChunk(chunkPos.x, chunkPos.y, chunkPos.z).m_Voxels.at(IndexLinear(vcpX, vcpY, vcpZ));
			return type;
		}
		else {
			return Voxel::Type::Empty;
		}
	}

	void Chunk::SetVoxel(int x, int y, int z, Voxel::Type type)
	{
		if (Inside(x, y, z))
			m_Voxels.at(IndexLinear(x, y, z)) = type;

		if (x == 0) {
			bool validChunk = m_World.Inside(m_ChunkPosition.x - 1, m_ChunkPosition.y, m_ChunkPosition.z);
			if (validChunk)
				m_World.GetChunk(m_ChunkPosition.x - 1, m_ChunkPosition.y, m_ChunkPosition.z).Update();
		}
		else if (x == SIZE - 1) {
			bool validChunk = m_World.Inside(m_ChunkPosition.x + 1, m_ChunkPosition.y, m_ChunkPosition.z);
			if (validChunk)
				m_World.GetChunk(m_ChunkPosition.x + 1, m_ChunkPosition.y, m_ChunkPosition.z).Update();
		}

		if (y == 0) {
			bool validChunk = m_World.Inside(m_ChunkPosition.x, m_ChunkPosition.y - 1, m_ChunkPosition.z);
			if (validChunk)
				m_World.GetChunk(m_ChunkPosition.x, m_ChunkPosition.y - 1, m_ChunkPosition.z).Update();
		}
		else if (y == SIZE - 1) {
			bool validChunk = m_World.Inside(m_ChunkPosition.x, m_ChunkPosition.y + 1, m_ChunkPosition.z);
			if (validChunk)
				m_World.GetChunk(m_ChunkPosition.x, m_ChunkPosition.y + 1, m_ChunkPosition.z).Update();
		}

		if (z == 0) {
			bool validChunk = m_World.Inside(m_ChunkPosition.x, m_ChunkPosition.y, m_ChunkPosition.z - 1);
			if (validChunk)
				m_World.GetChunk(m_ChunkPosition.x, m_ChunkPosition.y, m_ChunkPosition.z - 1).Update();
		}
		else if (z == SIZE - 1) {
			bool validChunk = m_World.Inside(m_ChunkPosition.x, m_ChunkPosition.y, m_ChunkPosition.z + 1);
			if (validChunk)
				m_World.GetChunk(m_ChunkPosition.x, m_ChunkPosition.y, m_ChunkPosition.z + 1).Update();
		}
	}

	int Chunk::IndexLinear(int x, int y, int z)
	{
		int a = SIZE * SIZE;	//Z * Y
		int b = SIZE;			//Z
		return a * x + b * y + z;
	}

	int Chunk::IndexLinear(const glm::ivec3& p)
	{
		int a = SIZE * SIZE;	//Z * Y
		int b = SIZE;			//Z
		return a * p.x + b * p.y + p.z;
	}

	bool Chunk::Inside(int x, int y, int z)
	{
		bool upper = x >= SIZE || y >= SIZE || z >= SIZE;
		bool lower = x < 0.0f || y < 0.0f || z < 0.0f;
		if (upper || lower)
			return false;
		return true;
	}

	bool Chunk::Inside(const glm::ivec3& p)
	{
		bool upper = p.x >= SIZE || p.y >= SIZE || p.z >= SIZE;
		bool lower = p.x < 0.0f || p.y < 0.0f || p.z < 0.0f;
		if (upper || lower)
			return false;
		return true;
	}

	void Chunk::AddVertex(const glm::vec3& pos, const glm::vec3& color, int side, int ao)
	{
		glm::vec3 worldPos = (pos * Voxel::SIZE + m_Position);
		m_VertexData.emplace_back(VertexData{ worldPos, color, side, ao});
	}

	int Chunk::VertexAO(const glm::ivec3& p, const glm::ivec3& d1, const glm::ivec3& d2)
	{
		bool insideA = Inside(p + d1);
		bool insideB = Inside(p + d2);
		bool insideC = Inside(p + d1 + d2);
		int indexA = IndexLinear(p + d1);
		int indexB = IndexLinear(p + d2);
		int indexC = IndexLinear(p + d1 + d2);
		bool adjA = false, adjB = false, adjC = false;

		if (insideA) {
			Voxel::Type typeA = m_Voxels.at(indexA);
			if (typeA != Voxel::Type::Empty)
				adjA = true;
		}

		if (insideB) {
			Voxel::Type typeB = m_Voxels.at(indexB);
			if (typeB != Voxel::Type::Empty)
				adjB = true;
		}

		if (insideC) {
			Voxel::Type typeC = m_Voxels.at(indexC);
			if (typeC != Voxel::Type::Empty)
				adjC = true;
		}

		if (adjA && adjB)
			return 5;
		return adjA + adjB + adjC;
	}
	
	void Chunk::AddFace(const glm::ivec3* vertices, const int* ao, const glm::ivec3& p, const glm::ivec3& d, const glm::vec3& color)
	{
		int side = Voxel::NO_SIDES;
		if (d.x == 1)
			side = Voxel::PX_SIDE;
		else if (d.x == -1)
			side = Voxel::NX_SIDE;
		if (d.y == 1)
			side = Voxel::PY_SIDE;
		else if (d.y == -1)
			side = Voxel::NY_SIDE;
		if (d.z == 1)
			side = Voxel::PZ_SIDE;
		else if (d.z == -1)
			side = Voxel::NZ_SIDE;

		if (ao[0] + ao[3] > ao[2] + ao[1]) {
			AddVertex({ vertices[0].x + p.x , vertices[0].y + p.y , vertices[0].z + p.z }, color, side, ao[0]);
			AddVertex({ vertices[2].x + p.x , vertices[2].y + p.y , vertices[2].z + p.z }, color, side, ao[2]);
			AddVertex({ vertices[1].x + p.x , vertices[1].y + p.y , vertices[1].z + p.z }, color, side, ao[1]);

			AddVertex({ vertices[1].x + p.x , vertices[1].y + p.y , vertices[1].z + p.z }, color, side, ao[1]);
			AddVertex({ vertices[2].x + p.x , vertices[2].y + p.y , vertices[2].z + p.z }, color, side, ao[2]);
			AddVertex({ vertices[3].x + p.x , vertices[3].y + p.y , vertices[3].z + p.z }, color, side, ao[3]);
		}																						  
		else {																					  
			AddVertex({ vertices[3].x + p.x , vertices[3].y + p.y , vertices[3].z + p.z }, color, side, ao[3]);
			AddVertex({ vertices[0].x + p.x , vertices[0].y + p.y , vertices[0].z + p.z }, color, side, ao[0]);
			AddVertex({ vertices[2].x + p.x , vertices[2].y + p.y , vertices[2].z + p.z }, color, side, ao[2]);
																								  
			AddVertex({ vertices[3].x + p.x , vertices[3].y + p.y , vertices[3].z + p.z }, color, side, ao[3]);
			AddVertex({ vertices[1].x + p.x , vertices[1].y + p.y , vertices[1].z + p.z }, color, side, ao[1]);
			AddVertex({ vertices[0].x + p.x , vertices[0].y + p.y , vertices[0].z + p.z }, color, side, ao[0]);
		}
	}

	void Chunk::CreateVoxelData() {
		//m_VertexData.reserve(36 * pow(SIZE, 3));
		m_VertexData.clear();
		glm::vec3 color = glm::vec3{ 0.16078f,  0.19608f,  0.25490f, };

		for (int x = 0; x < SIZE; x++) {
			for (int y = 0; y < SIZE; y++) {
				for (int z = 0; z < SIZE; z++) {
					int voxelIndex = IndexLinear(x, y, z);
					if (m_Voxels.at(voxelIndex) != Voxel::Type::Empty) {
						
						if (GetVoxel(x - 1, y, z) == Voxel::Type::Empty) {
							glm::ivec3 vert[4] = {
								{0, 0, 0},
								{0, 1, 0},
								{0, 0, 1},
								{0, 1, 1},
							};
							int ao[4] = {
								VertexAO({ x - 1, y, z }, { 0, -1, 0 }, { 0, 0, -1 }),
								VertexAO({ x - 1, y, z }, { 0,  1, 0 }, { 0, 0, -1 }),
								VertexAO({ x - 1, y, z }, { 0, -1, 0 }, { 0, 0,  1 }),
								VertexAO({ x - 1, y, z }, { 0,  1, 0 }, { 0, 0,  1 }),
							};
							AddFace(vert, ao, { x, y, z }, { -1, 0, 0 }, color);
						}

						if (GetVoxel(x + 1, y, z) == Voxel::Type::Empty) {
							glm::ivec3 vert[4] = {
								{1, 1, 1},
								{1, 1, 0},
								{1, 0, 1},
								{1, 0, 0},
							};
							int ao[4] = {
								VertexAO({ x + 1, y, z }, { 0,  1, 0 }, { 0, 0,  1 }),
								VertexAO({ x + 1, y, z }, { 0,  1, 0 }, { 0, 0, -1 }),
								VertexAO({ x + 1, y, z }, { 0, -1, 0 }, { 0, 0,  1 }),
								VertexAO({ x + 1, y, z }, { 0, -1, 0 }, { 0, 0, -1 }),
							};
							AddFace(vert, ao, { x, y, z }, { 1, 0, 0 }, color);
						}

						if (GetVoxel(x, y - 1, z) == Voxel::Type::Empty) {
							glm::ivec3 face[4] = {
								{1, 0, 1},
								{1, 0, 0},
								{0, 0, 1},
								{0, 0, 0},
							};
							int ao[4] = {
								VertexAO({ x, y - 1, z }, {  1, 0, 0 }, { 0, 0,  1 }),
								VertexAO({ x, y - 1, z }, {  1, 0, 0 }, { 0, 0, -1 }),
								VertexAO({ x, y - 1, z }, { -1, 0, 0 }, { 0, 0,  1 }),
								VertexAO({ x, y - 1, z }, { -1, 0, 0 }, { 0, 0, -1 }),
							};
							AddFace(face, ao, { x, y, z }, { 0, -1, 0 }, color);
						}
						
						if (GetVoxel(x, y + 1, z) == Voxel::Type::Empty) {
							glm::ivec3 face[4] = {
								{0, 1, 0},
								{1, 1, 0},
								{0, 1, 1},
								{1, 1, 1},
							};
							int ao[4] = {
								VertexAO({ x, y + 1, z }, { -1, 0, 0 }, { 0, 0, -1 }),
								VertexAO({ x, y + 1, z }, {  1, 0, 0 }, { 0, 0, -1 }),
								VertexAO({ x, y + 1, z }, { -1, 0, 0 }, { 0, 0,  1 }),
								VertexAO({ x, y + 1, z }, {  1, 0, 0 }, { 0, 0,  1 }),
							};
							AddFace(face, ao, { x, y, z }, { 0, 1, 0 }, color);
						}
						
						if (GetVoxel(x, y, z - 1) == Voxel::Type::Empty) {
							glm::ivec3 face[4] = {
								{0, 0, 0},
								{1, 0, 0},
								{0, 1, 0},
								{1, 1, 0},
							};
							int ao[4] = {
								VertexAO({ x, y, z - 1 }, { -1, 0, 0 }, { 0, -1, 0 }),
								VertexAO({ x, y, z - 1 }, {  1, 0, 0 }, { 0, -1, 0 }),
								VertexAO({ x, y, z - 1 }, { -1, 0, 0 }, { 0,  1, 0 }),
								VertexAO({ x, y, z - 1 }, {  1, 0, 0 }, { 0,  1, 0 }),
							};
							AddFace(face, ao, { x, y, z }, { 0, 0, -1 }, color);
						}

						if (GetVoxel(x, y, z + 1) == Voxel::Type::Empty) {
							glm::ivec3 face[4] = {
								{1, 1, 1},
								{1, 0, 1},
								{0, 1, 1},
								{0, 0, 1},
							};
							int ao[4] = {
								VertexAO({ x, y, z + 1 }, {  1, 0, 0 }, { 0,  1, 0 }),
								VertexAO({ x, y, z + 1 }, {  1, 0, 0 }, { 0, -1, 0 }),
								VertexAO({ x, y, z + 1 }, { -1, 0, 0 }, { 0,  1, 0 }),
								VertexAO({ x, y, z + 1 }, { -1, 0, 0 }, { 0, -1, 0 }),
							};
							AddFace(face, ao, { x, y, z }, { 0, 0, 1 }, color);
						}
					}
				}
			}
		}
	}
}