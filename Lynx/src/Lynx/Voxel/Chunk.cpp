#include "Lynxpch.h"
#include "Chunk.h"
#include "World.h"
#include "Lynx/Utility/BitMask.h"
#include "Lynx/Utility/Packing.h"
#include "glad/glad.h"

namespace Lynx {
	Chunk::Chunk(World& world, unsigned int x, unsigned int y, unsigned int z) : m_World(world)
	{
		m_ChunkPosition = { x, y, z };
		m_Position = { x * Chunk::SIZE * Voxel::SIZE, y * Chunk::SIZE * Voxel::SIZE, z * Chunk::SIZE * Voxel::SIZE };
		m_Voxels.resize(SIZE * SIZE * SIZE);
	}

	void Chunk::CreateMesh()
	{
		CreateVoxelData();

		m_VA = VertexArray::Create();
		m_VB = VertexBuffer::Create(m_VertexData.data(), m_VertexData.size() * sizeof(VertexData));
		m_VB->SetLayout({
			{ Lynx::ShaderDataType::Float3, "a_Position" },
			{ Lynx::ShaderDataType::PackedInt, "a_Color" },
			{ Lynx::ShaderDataType::UInt, "a_SideAndAO" }
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

	Voxel2 Chunk::GetVoxel(int x, int y, int z) {
		glm::vec3 voxelWorldPos = { m_ChunkPosition.x * SIZE + x, m_ChunkPosition.y * SIZE + y, m_ChunkPosition.z * SIZE + z };
		glm::ivec3 chunkPos = glm::floor(voxelWorldPos / (float)Chunk::SIZE);
		bool validChunk = m_World.Inside(chunkPos.x, chunkPos.y, chunkPos.z);
		if (validChunk) {
			int vcpX = (int)voxelWorldPos.x % Chunk::SIZE;
			int vcpY = (int)voxelWorldPos.y % Chunk::SIZE;
			int vcpZ = (int)voxelWorldPos.z % Chunk::SIZE;
			Voxel2 v = m_World.GetChunk(chunkPos.x, chunkPos.y, chunkPos.z).m_Voxels.at(IndexLinear(vcpX, vcpY, vcpZ));
			return v;
		}
		else {
			return Voxel2();
		}
	}

	void Chunk::SetVoxelType(int x, int y, int z, Voxel::Type type)
	{
		//Set voxel type
		if (Inside(x, y, z))
			m_Voxels.at(IndexLinear(x, y, z)).m_Type = type;

		//Chunk border check
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

	void Chunk::SetVoxelColor(int x, int y, int z, const glm::vec4 color)
	{
		if (Inside(x, y, z))
			m_Voxels.at(IndexLinear(x, y, z)).SetColor(color);
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

	int Chunk::VertexAO(const glm::ivec3& p, const glm::ivec3& d1, const glm::ivec3& d2)
	{
		bool adjA2 = false, adjB2 = false, adjC2 = false;
		glm::ivec3 a = p + d1;
		Voxel::Type typeA2 = GetVoxel(a.x, a.y, a.z).m_Type;
		if (typeA2 != Voxel::Type::Empty)
			adjA2 = true;
		glm::ivec3 b = p + d2;
		Voxel::Type typeB2 = GetVoxel(b.x, b.y, b.z).m_Type;
		if (typeB2 != Voxel::Type::Empty)
			adjB2 = true;
		glm::ivec3 c = p + d1 + d2;
		Voxel::Type typeC2 = GetVoxel(c.x, c.y, c.z).m_Type;
		if (typeC2 != Voxel::Type::Empty)
			adjC2 = true;

		if (adjA2 && adjB2)
			return 5;
		return adjA2 + adjB2 + adjC2;
	}

	void Chunk::AddVertex(const glm::vec3& pos, uint32_t color, int side, int ao)
	{
		glm::vec3 worldPos = (pos * Voxel::SIZE + m_Position);
		
		m_VertexData.emplace_back(VertexData{ worldPos, color, pack2x4(side, ao) });
	}
	
	void Chunk::AddFace(const glm::ivec3* vertices, const int* ao, const glm::ivec3& p, const glm::ivec3& d, uint32_t color)
	{
		int side = 0;
		if (d.x == 1)
			side = 0;
		else if (d.x == -1)
			side = 1;
		if (d.y == 1)
			side = 2;
		else if (d.y == -1)
			side = 3;
		if (d.z == 1)
			side = 4;
		else if (d.z == -1)
			side = 5;

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
		glm::vec3 color = glm::vec3{ 1.0f,  1.0f,  1.0f }; //glm::vec3{ 0.16078f,  0.19608f,  0.25490f };

		for (int x = 0; x < SIZE; x++) {
			for (int y = 0; y < SIZE; y++) {
				for (int z = 0; z < SIZE; z++) {
					int voxelIndex = IndexLinear(x, y, z);
					Voxel2 voxel = m_Voxels.at(voxelIndex);
					if (voxel.m_Type != Voxel::Type::Empty) {
						
						if (GetVoxel(x - 1, y, z).m_Type == Voxel::Type::Empty) {
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
							AddFace(vert, ao, { x, y, z }, { -1, 0, 0 }, voxel.GetColor());
						}

						if (GetVoxel(x + 1, y, z).m_Type == Voxel::Type::Empty) {
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
							AddFace(vert, ao, { x, y, z }, { 1, 0, 0 }, voxel.GetColor());
						}

						if (GetVoxel(x, y - 1, z).m_Type == Voxel::Type::Empty) {
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
							AddFace(face, ao, { x, y, z }, { 0, -1, 0 }, voxel.GetColor());
						}
						
						if (GetVoxel(x, y + 1, z).m_Type == Voxel::Type::Empty) {
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
							AddFace(face, ao, { x, y, z }, { 0, 1, 0 }, voxel.GetColor());
						}
						
						if (GetVoxel(x, y, z - 1).m_Type == Voxel::Type::Empty) {
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
							AddFace(face, ao, { x, y, z }, { 0, 0, -1 }, voxel.GetColor());
						}

						if (GetVoxel(x, y, z + 1).m_Type == Voxel::Type::Empty) {
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
							AddFace(face, ao, { x, y, z }, { 0, 0, 1 }, voxel.GetColor());
						}
					}
				}
			}
		}
	}
}