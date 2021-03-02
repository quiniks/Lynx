#include "Lynxpch.h"
#include "Grid.h"
#include "glad/glad.h"

namespace Lynx {
	Grid::Grid()
	{

	}

	void Grid::Init(float cellSize, int gridSize, glm::vec3 offset)
	{
		m_CellSize = cellSize;
		m_GridSize = gridSize;
		float y = 0.0f;

		for (int x = 0; x <= m_GridSize; x++) {
			m_VertexData.emplace_back(offset.x + x * cellSize, offset.y, offset.z + 0.0f);
			m_VertexData.emplace_back(offset.x + x * cellSize, offset.y, offset.z + gridSize * cellSize);
		}

		for (int z = 0; z <= m_GridSize; z++) {
			m_VertexData.emplace_back(offset.x + 0.0f, offset.y + y, offset.z + z * cellSize);
			m_VertexData.emplace_back(offset.x + gridSize * cellSize, offset.y + y, offset.z + z * cellSize);
		}

		m_VA = VertexArray::Create();
		m_VB = VertexBuffer::Create(&m_VertexData[0], m_VertexData.size() * sizeof(glm::vec3));
		m_VB->SetLayout({ { Lynx::ShaderDataType::Float3, "a_Position" } });
		m_VA->AddVertexBuffer(m_VB);
	}

	void Grid::Render()
	{
		m_VA->Bind();
		glDrawArrays(GL_LINES, 0, (GLsizei)m_VertexData.size());
	}
}
