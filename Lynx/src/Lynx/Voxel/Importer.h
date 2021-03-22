#pragma once
#include "Lynx/Voxel/Formats.h"
#include "Lynx/Voxel/Voxel.h"
#include "Lynx/Voxel/Chunk.h"

namespace Lynx {
	class Importer {
	public:
		static XRAW XRawImport(const std::string& file);
		static std::vector<Chunk> XRAWToVoxel(const XRAW& xrawData);
	private:
		static int IndexLinear(int x, int y, int z, int size);
		static int IndexLinear(int x, int y, int z, int width, int height);
	};
}