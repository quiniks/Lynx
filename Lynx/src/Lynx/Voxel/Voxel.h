#pragma once
#include "Lynx/Utility/BitMask.h"
#include "Lynx/Detail/glm.h"

namespace Lynx {
	namespace Voxel {
		enum class Type {
			Empty = 0,
			Solid,
		};

		constexpr float SIZE = 0.2f;
		constexpr int SIX_SIDES = 63;
		constexpr int NO_SIDES = 0;
		constexpr int PX_SIDE = BIT(1);
		constexpr int NX_SIDE = BIT(0);
		constexpr int PY_SIDE = BIT(3);
		constexpr int NY_SIDE = BIT(2);
		constexpr int PZ_SIDE = BIT(5);
		constexpr int NZ_SIDE = BIT(4);
	}

	class Voxel2 {
	public:
		void SetColor(const glm::vec4& color) {
			m_Color = glm::packU3x10_1x2(color);
		};
		uint32_t GetColor() {
			return m_Color;
		}
	public:
		Voxel::Type m_Type = Voxel::Type::Empty;
	private:
		uint32_t m_Color = 0;
	};
}