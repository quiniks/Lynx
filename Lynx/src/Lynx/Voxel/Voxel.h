#pragma once
#include "Lynx/Utility/BitMask.h"

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
}