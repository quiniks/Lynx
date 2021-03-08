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
			{ Lynx::ShaderDataType::Float3, "a_Normal" },
			{ Lynx::ShaderDataType::Int, "a_AO" }
			});
		m_VA->AddVertexBuffer(m_VB);

		CreateVoxelData();
		//LX_INFO("init size: {0}", m_VertexData.size());
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

	int Chunk::AddVertex(const glm::vec3& pos, const glm::vec3& color, const glm::vec3& normal, int ao)
	{
		glm::vec3 worldPos = (pos * Voxel::SIZE + m_Position);
		m_VertexData.emplace_back(VertexData{ worldPos, color, normal, ao});
		//LX_INFO("worldPos: {0}, {1}, {2}", worldPos.x, worldPos.y, worldPos.z);
		return 0;//(int)m_VertexData.size() - 1;
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
		if (ao[0] + ao[3] > ao[2] + ao[1]) {
			AddVertex({ vertices[0].x + p.x , vertices[0].y + p.y , vertices[0].z + p.z }, color, { d.x, d.y, d.z }, ao[0]);
			AddVertex({ vertices[2].x + p.x , vertices[2].y + p.y , vertices[2].z + p.z }, color, { d.x, d.y, d.z }, ao[2]);
			AddVertex({ vertices[1].x + p.x , vertices[1].y + p.y , vertices[1].z + p.z }, color, { d.x, d.y, d.z }, ao[1]);

			AddVertex({ vertices[1].x + p.x , vertices[1].y + p.y , vertices[1].z + p.z }, color, { d.x, d.y, d.z }, ao[1]);
			AddVertex({ vertices[2].x + p.x , vertices[2].y + p.y , vertices[2].z + p.z }, color, { d.x, d.y, d.z }, ao[2]);
			AddVertex({ vertices[3].x + p.x , vertices[3].y + p.y , vertices[3].z + p.z }, color, { d.x, d.y, d.z }, ao[3]);
		}
		else {
			AddVertex({ vertices[3].x + p.x , vertices[3].y + p.y , vertices[3].z + p.z }, color, { d.x, d.y, d.z }, ao[3]);
			AddVertex({ vertices[0].x + p.x , vertices[0].y + p.y , vertices[0].z + p.z }, color, { d.x, d.y, d.z }, ao[0]);
			AddVertex({ vertices[2].x + p.x , vertices[2].y + p.y , vertices[2].z + p.z }, color, { d.x, d.y, d.z }, ao[2]);

			AddVertex({ vertices[3].x + p.x , vertices[3].y + p.y , vertices[3].z + p.z }, color, { d.x, d.y, d.z }, ao[3]);
			AddVertex({ vertices[1].x + p.x , vertices[1].y + p.y , vertices[1].z + p.z }, color, { d.x, d.y, d.z }, ao[1]);
			AddVertex({ vertices[0].x + p.x , vertices[0].y + p.y , vertices[0].z + p.z }, color, { d.x, d.y, d.z }, ao[0]);
		}
	}

	void Chunk::CreateVoxelData() {
		m_VertexData.reserve((uint32_t)(pow(SIZE, 3) * 8));

		glm::vec3 color = { 1.0f, 1.0f, 1.0f };// (glm::vec3)m_ChunkPosition / (glm::vec3)m_World.SIZE + glm::vec3{ 0.8f, 0.2f, 0.3f };

		for (int x = 0; x < SIZE; x++) {
			for (int y = 0; y < SIZE; y++) {
				for (int z = 0; z < SIZE; z++) {
					int voxelIndex = IndexLinear(x, y, z);
					if (m_Voxels.at(voxelIndex) != Voxel::Type::Empty) {
						
						if (x == 0 || (x > 0 && m_Voxels.at(IndexLinear(x - 1, y, z)) == Voxel::Type::Empty)) {
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

						if (x == SIZE - 1 || (x < SIZE - 1 && m_Voxels.at(IndexLinear(x + 1, y, z)) == Voxel::Type::Empty)) {
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

						if (y == 0 || (y > 0 && m_Voxels.at(IndexLinear(x, y - 1, z)) == Voxel::Type::Empty)) {
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
						
						if (y == SIZE - 1 || (y < SIZE - 1 && m_Voxels.at(IndexLinear(x, y + 1, z)) == Voxel::Type::Empty)) {
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
						
						if (z == 0 || (z > 0 && m_Voxels.at(IndexLinear(x, y, z - 1)) == Voxel::Type::Empty)) {
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

						if (z == SIZE - 1 || (z < SIZE - 1 && m_Voxels.at(IndexLinear(x, y, z + 1)) == Voxel::Type::Empty)) {
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