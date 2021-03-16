#pragma once
#include <cstdint>

namespace Lynx {
	//Packs 2, 4 bit, integers into a uint8_t
	uint8_t pack2x4(unsigned l, unsigned r) {
		return l << 4 | r;
	}
}