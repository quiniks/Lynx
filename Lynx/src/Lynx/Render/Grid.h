#pragma once
#include "Lynx/Render/VertexArray.h"
#include "Lynx/Detail/glm.h"

namespace Lynx {
	class Grid {
	public:
		Grid();
		void Init(float cellSize, int gridSize, glm::vec3 offset);
		void Render();
	private:
		float m_CellSize = 0;
		int m_GridSize = 0;

		std::vector<glm::vec3> m_VertexData;
		Ref<VertexArray> m_VA = nullptr;
		Ref<VertexBuffer> m_VB = nullptr;
	};
}
