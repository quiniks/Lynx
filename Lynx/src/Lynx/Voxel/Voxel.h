#pragma once

namespace Lynx {
	namespace Voxel {
		enum class Type {
			Empty = 0,
			Solid,
		};
		constexpr float SIZE = 0.2f;
		constexpr int SIX_SIDES = 63;
		constexpr int NO_SIDES = 0;
	}
}